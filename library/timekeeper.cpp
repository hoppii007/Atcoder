#pragma once
/**
 * @brief 経過時間の計測（ミリ秒）
 *
 *   tk.start()  … 計測開始
 *   tk.now()    … start() からの経過ミリ秒（double、1ms 単位）
 *
 * @code
 * time_keeper tk;
 * tk.start();
 * while(tk.now() < 1900){ ... }
 * @endcode
 */
class time_keeper {
public:
    chrono::system_clock::time_point  start_point, end_point;
    void start(){
        start_point = chrono::system_clock::now();
    }
    double now(){
        end_point = chrono::system_clock::now();
        double elapsed = chrono::duration_cast<std::chrono::milliseconds>(end_point-start_point).count();
        return elapsed;
    }
};
