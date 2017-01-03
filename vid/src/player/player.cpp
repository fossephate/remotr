#include "player.h"
#include <algorithm>
#include <chrono>
#include <iostream>

#include "SDL2\SDL_mixer.h"

extern "C" {
	#include <libavutil/time.h>
	#include <libavutil/imgutils.h>
}

// init
Player::Player(const std::string &file_name) :
	demuxer_{new Demuxer{file_name}},
	video_decoder_{new VideoDecoder{demuxer_->video_codec_context()}},
	format_converter_{new FormatConverter{video_decoder_->width(), video_decoder_->height(), video_decoder_->pixel_format(), AV_PIX_FMT_YUV420P}},
	display_{new Display{video_decoder_->width(), video_decoder_->height()}},
	timer_{new Timer},
	packet_queue_{new PacketQueue{queue_size_}},
	frame_queue_{new FrameQueue{queue_size_}} {

	std::string mp3filename = file_name.substr(0, file_name.length()-1) + "3";

	int result = 0;
	int flags = MIX_InitFlags::MIX_INIT_MP3;
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		printf("Failed to init SDL\n");
		exit(1);
	}

	if (flags != (result = Mix_Init(flags))) {
		printf("Could not initialize mixer (result: %d).\n", result);
		printf("Mix_Init: %s\n", Mix_GetError());
		exit(1);
	}

	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
	Mix_Music *music = Mix_LoadMUS(mp3filename.c_str());
	Mix_PlayMusic(music, 1);

	/*while (!SDL_QuitRequested()) {
		SDL_Delay(250);
	}*/

	//Mix_FreeMusic(music);
	//SDL_Quit();

}

Player::~Player() {
	frame_queue_->quit();
	packet_queue_->quit();

	for (auto &stage : stages_) {
		stage.join();
	}
}

void Player::operator()() {
	stages_.emplace_back(&Player::demultiplex, this);
	stages_.emplace_back(&Player::decode_video, this);
	video();
}

void Player::demultiplex() {
	try {
		for (;;) {
			// Create AVPacket
			std::unique_ptr<AVPacket, std::function<void(AVPacket*)>> packet{
				new AVPacket, [](AVPacket* p){ av_packet_unref(p); delete p; }};
			av_init_packet(packet.get());
			packet->data = nullptr;

			// Read frame into AVPacket
			if (!(*demuxer_)(*packet)) {
				packet_queue_->finished();
				break;
			}

			// Move into queue if first video stream
			if (packet->stream_index == demuxer_->video_stream_index()) {
				if (!packet_queue_->push(move(packet))) {
					break;
				}
			}
		}
	} catch (std::exception &e) {
		std::cerr << "Demuxing error: " << e.what() << std::endl;
		exit(1);
	}
}

void Player::decode_video() {

	const AVRational microseconds = {1, 1000000};

	try {
		for (;;) {
			// Create AVFrame and AVQueue
			std::unique_ptr<AVFrame, std::function<void(AVFrame*)>> frame_decoded{
				av_frame_alloc(), [](AVFrame* f){ av_frame_free(&f); }};
			std::unique_ptr<AVPacket, std::function<void(AVPacket*)>> packet{
				nullptr, [](AVPacket* p){ av_packet_unref(p); delete p; }};

			// Read packet from queue
			if (!packet_queue_->pop(packet)) {
				frame_queue_->finished();
				break;
			}

			// Decode packet
			int finished_frame;
			(*video_decoder_)(
				frame_decoded.get(), finished_frame, packet.get());

			// If a whole frame has been decoded,
			// adjust time stamps and add to queue
			if (finished_frame) {
				frame_decoded->pts = av_rescale_q(
					frame_decoded->pkt_dts,
					demuxer_->time_base(),
					microseconds);

				std::unique_ptr<AVFrame, std::function<void(AVFrame*)>> frame_converted{
					av_frame_alloc(),
					[](AVFrame* f){ av_free(f->data[0]); }};
				if (av_frame_copy_props(frame_converted.get(),
				    frame_decoded.get()) < 0) {
					throw std::runtime_error("Copying frame properties");
				}
				if (av_image_alloc(
					frame_converted->data, frame_converted->linesize,
					video_decoder_->width(), video_decoder_->height(),
					video_decoder_->pixel_format(), 1) < 0) {
					throw std::runtime_error("Allocating picture");
				}	
				(*format_converter_)(
					frame_decoded.get(), frame_converted.get());

				if (!frame_queue_->push(move(frame_converted))) {
					break;
				}
			}
		}
	} catch (std::exception &e) {
		std::cerr << "Decoding error: " <<  e.what() << std::endl;
		exit(1);
	}

}

void Player::video() {
	try {
		int64_t last_pts = 0;

		for (uint64_t frame_number = 0;; ++frame_number) {

			display_->input();

			//SDL_Window * win = (SDL_Window *)&display_->window_;
			//SDL_MaximizeWindow(win);

			if (display_->get_quit()) {
				break;

			} else if (display_->get_play()) {
				std::unique_ptr<AVFrame, std::function<void(AVFrame*)>> frame{
					nullptr, [](AVFrame* f){ av_frame_free(&f); }};
				if (!frame_queue_->pop(frame)) {
					break;
				}

				if (frame_number) {
					const int64_t frame_delay = frame->pts - last_pts;
					last_pts = frame->pts;
					timer_->wait(frame_delay);

				} else {
					last_pts = frame->pts;
					timer_->update();
				}

				display_->refresh(
					{frame->data[0], frame->data[1], frame->data[2]},
					{static_cast<size_t>(frame->linesize[0]),
					 static_cast<size_t>(frame->linesize[1]),
					 static_cast<size_t>(frame->linesize[2])});

			} else {
				std::chrono::milliseconds sleep(10);
				std::this_thread::sleep_for(sleep);
				timer_->update();
			}
		}
	} catch (std::exception &e) {
		std::cerr << "Display error: " <<  e.what() << std::endl;
		exit(1);
	}
}
