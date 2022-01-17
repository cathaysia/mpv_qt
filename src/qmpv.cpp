#include "qmpv.hpp"

#include <stdexcept>
MpvPlayer::MpvPlayer(QWidget* container, QObject* parent) : QObject(parent), container_(container), mpv_(mpv_create()) {
    if(!mpv_) throw std::runtime_error("创建 mpv 实例失败");

    container_->setAttribute(Qt::WA_DontCreateNativeAncestors);
    container_->setAttribute(Qt::WA_NativeWindow);

    // 嵌入 mpv
    int64_t wid = container_->winId();
    mpv_set_option(mpv_, "wid", MPV_FORMAT_INT64, &wid);

    // 控制 mpv 事件
    connect(this, &MpvPlayer::onMpvEventChanged, this, &MpvPlayer::handleMpvEvent, Qt::QueuedConnection);
    mpv_set_wakeup_callback(
        mpv_,
        [](void* obj) {    // 这个函数不能执行 mpv_wait_event
            auto* player = static_cast<MpvPlayer*>(obj);
            emit  player->onMpvEventChanged();
        },
        this);

    if(mpv_initialize(mpv_) < 0) throw std::runtime_error("mpv 初始化失败");
}

MpvPlayer::~MpvPlayer() {
    if(mpv_) mpv_terminate_destroy(mpv_);
    mpv_ = nullptr;
}

mpv_handle* MpvPlayer::getMpvHandle() {
    return mpv_;
}

void MpvPlayer::handleMpvEvent() {
    auto* event = mpv_wait_event(mpv_, 0);
    if(event->event_id == MPV_EVENT_NONE) return;
    if(event->event_id == MPV_EVENT_SHUTDOWN) {
        mpv_terminate_destroy(mpv_);
        mpv_ = nullptr;
    }
}

void MpvPlayer::setUrl(const QString& url) {
    this->url_ = url.toUtf8();
}

void MpvPlayer::play() {
    const char* args[] = { "loadfile", url_.data(), nullptr };
    mpv_command_async(mpv_, 0, args);
}
