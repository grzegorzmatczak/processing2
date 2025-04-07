#pragma once

namespace cv
{
    class Mat;
}

struct _data;

class QThread;
class QString;
class QJsonObject;
class QRandomGenerator;
class QJsonArray;

namespace logger
{
    class Logger;
    class LoggerThread;
}

constexpr auto GRAPH{"Graph"};
constexpr auto NAME{"Name"};
constexpr auto ID{"Id"};
constexpr auto COPY_SIGNAL{"Signal"};
constexpr auto TYPE{"Type"};
constexpr auto NEXT{"Next"};
constexpr auto PREV{"Prev"};
constexpr auto CONFIG{"Config"};
constexpr auto WIDTH{"Width"};
constexpr auto HEIGHT{"Height"};
constexpr auto X{"X"};
constexpr auto Y{"Y"};
constexpr auto TID{"ThreadId"};
constexpr auto TPREV{"ThreadPrev"};
constexpr auto TNEXT{"ThreadNext"};



