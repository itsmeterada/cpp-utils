#include <iostream>
#include <cstdio>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <SDL2/SDL.h>
}

int main(int argc, char* argv[]) {
  // 動画のパスを指定
  const char* path = "sample.mp4";

  // FFmpegの初期化
  av_register_all();

  // 動画のフォーマット情報を格納するAVFormatContextを作成
  AVFormatContext* formatContext = avformat_alloc_context();
  if (!formatContext) {
    std::cerr << "Could not allocate format context." << std::endl;
    return -1;
  }

  // 動画のストリーム情報を取得
  if (avformat_open_input(&formatContext, path, nullptr, nullptr) < 0) {
    std::cerr << "Could not open input file: " << path << std::endl;
    return -1;
  }
  if (avformat_find_stream_info(formatContext, nullptr) < 0) {
    std::cerr << "Could not retrieve stream information." << std::endl;
    return -1;
  }

  // 動画ストリームのインデックスを取得
  int videoStreamIndex = -1;
  for (int i = 0; i < formatContext->nb_streams; i++) {
    if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
          videoStreamIndex = i;
          break;
    }
  }
  if (videoStreamIndex == -1) {
    std::cerr << "Could not find video stream." << std::endl;
    return -1;
  }

  // 動画のコーデック情報を格納するAVCodecContextを作成
  AVCodecContext* codecContext = avcodec_alloc_context3(nullptr);
  if (!codecContext) {
    std::cerr << "Could not allocate codec context." << std::endl;
    return -1;
  }
  if (avcodec_parameters_to_context(codecContext, formatContext->streams[videoStreamIndex]->codecpar) < 0) {
    std::cerr << "Could not copy codec parameters to codec context." << std::endl;
    return -1;
  }

  // 動画のコーデックを探して、コーデックコンテキストに関連付け
  AVCodec* codec = avcodec_find_decoder(codecContext->codec_id);
  if (!codec) {
    std::cerr << "Could not find decoder." << std::endl;
    return -1;
  }
  if (avcodec_open2(codecContext, codec, nullptr) < 0) {
    std::cerr << "Could not open codec." << std::endl;
    return -1;
  } 

  // SDLの初期化
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
    std::cerr << "Could not initialize SDL. Error: " << SDL_GetError() << std::endl;
    return -1;
  }

  // 動画のフレームを格納するAVFrameを作成
  AVFrame* frame = av_frame_alloc();
  if (!frame) {
    std::cerr << "Could not allocate frame." << std::endl;
    return -1;
  }

  // 画面に表示するためのSDL_Textureを作成
  SDL_Window* window = SDL_CreateWindow("FFmpeg & SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, codecContext->width, codecContext->height, SDL_WINDOW_SHOWN);
  if (!window) {
    std::cerr << "Could not create window. Error: " << SDL_GetError() << std::endl;
    return -1;
  }
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    std::cerr << "Could not create renderer. Error: " << SDL_GetError() << std::endl;
    return -1;
  }
  SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, codecContext->width, codecContext->height);
  if (!texture) {
    std::cerr << "Could not create texture. Error: " << SDL_GetError() << std::endl;
    return -1;
  }

  // 動画のパケットを格納するAVPacketを作成
  AVPacket packet;
  av_init_packet(&packet);
  packet.data = nullptr;
  packet.size = 0;

  // 動画のフレームを読み込んで表示
  int exit_program = 0;
  SDL_Event event;
  while (!exit_program) {
    if (av_read_frame(formatContext, &packet) >= 0) {
      // 動画ストリームのフレームだけ処理する
      if (packet.stream_index == videoStreamIndex) {
        int response = avcodec_send_packet(codecContext, &packet);
        if (response < 0) {
          std::cerr << "Could not send packet to codec context. Error: " << response << std::endl;
          return -1;
        }
        while (response >= 0) {
          response = avcodec_receive_frame(codecContext, frame);
          if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
            break;
          } else if (response < 0) {
            std::cerr << "Could not receive frame from codec context. Error: " << response << std::endl;
            return -1;
          }
          SDL_UpdateYUVTexture(texture, nullptr, frame->data[0], frame->linesize[0], frame->data[1], frame->linesize[1], frame->data[2], frame->linesize[2]);
          SDL_RenderClear(renderer);
          SDL_RenderCopy(renderer, texture, nullptr, nullptr);
          SDL_RenderPresent(renderer);
        }
      } 
    } else {
        //av_seek_frame(formatContext, -1, 0, AVSEEK_FLAG_BACKWARD);
        av_seek_frame(formatContext, videoStreamIndex, 0, AVSEEK_FLAG_BACKWARD);
        avcodec_flush_buffers(codecContext);
    }

    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) {
      exit_program = 1;
    }

    av_packet_unref(&packet);
  }

  // 後始末
  av_frame_free(&frame);
  avcodec_close(codecContext);
  avformat_close_input(&formatContext);
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}