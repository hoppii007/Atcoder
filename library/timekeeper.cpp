#pragma once
/**
 * @brief 経過時間の計測（ミリ秒）
 *
 *   tk.start()  … 計測開始
 *   tk.now()    … start() からの経過ミリ秒（double、1ms 単位）
 *
 * @code
 * TimeKeeper tk;
 * tk.start();
 * while(tk.now() < 1900){ ... }
 * @endcode
 */
class TimeKeeper {
public:
    chrono::system_clock::time_point  startPoint, endPoint;
    void start(){
        startPoint = chrono::system_clock::now();
    }
    double now(){
        endPoint = chrono::system_clock::now();
        double elapsed = chrono::duration_cast<std::chrono::milliseconds>(endPoint-startPoint).count();
        return elapsed;
    }
};
