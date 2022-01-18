#include "qmpv.hpp"

#include <QDebug>

#include <stdexcept>
MpvPlayer::MpvPlayer(QWidget* container, QObject* parent)
    : QObject(parent)
    , timer_(new QTimer)
    , container_(container)
    , mpv_(mpv_create()) {
    if(!mpv_) throw std::runtime_error("创建 mpv 实例失败");

    container_->setAttribute(Qt::WA_DontCreateNativeAncestors);
    container_->setAttribute(Qt::WA_NativeWindow);

    // 嵌入 mpv
    int64_t wid = container_->winId();
    mpv_set_option(mpv_, "wid", MPV_FORMAT_INT64, &wid);

    initMpvProps();
    // 控制 mpv 事件
    connect(this,
            &MpvPlayer::onMpvEventChanged,
            this,
            &MpvPlayer::handleMpvEvent,
            Qt::QueuedConnection);
    mpv_set_wakeup_callback(
        mpv_,
        [](void* obj) {    // 这个函数不能执行 mpv_wait_event
            auto* player = static_cast<MpvPlayer*>(obj);
            emit  player->onMpvEventChanged();
        },
        this);
    // 定期处理 mpv 事件
    timer_->setInterval(500);
    connect(timer_, &QTimer::timeout, this, &MpvPlayer::handleMpvEvent);
    timer_->start();

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
    if(event->event_id == MPV_EVENT_PROPERTY_CHANGE) {
        auto* pros = static_cast<mpv_event_property*>(event->data);
        // 播放位置发生了变化
        if(strcmp(pros->name, "time-pos") == 0) {
            if(pros->format == MPV_FORMAT_DOUBLE)
                emit this->positioinChanged(*static_cast<double*>(pros->data));
            // 属性不可达，可能音频已经停止
            if(pros->format == MPV_FORMAT_NONE) emit this->positioinChanged(0);
        }
    }
}

void MpvPlayer::initMpvProps() {
    mpv_observe_property(mpv_, 0, "time-pos", MPV_FORMAT_DOUBLE);
}

void MpvPlayer::setPosition(double position) {
    mpv_set_property(mpv_, "time-pos", MPV_FORMAT_DOUBLE, &position);
}

double MpvPlayer::duration() const {
    double data = 0;
    mpv_get_property(mpv_, "duration", MPV_FORMAT_DOUBLE, &data);
    return data;
}

void MpvPlayer::addMedia(const QString& url) {
    const char* args[] = { "loadfile", url.toUtf8().data(), "append-play",
                           nullptr };
    mpv_command_async(mpv_, 0, args);
}
