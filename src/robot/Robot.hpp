#ifndef ROBOT_H
#define ROBOT_H

class Robot {
    private:
        static void py_init(const char* robot_ip , bool bypass_robot);
        const bool BYPASS_ROBOT;
        bool block_ended();
    public:
        Robot(const char* robot_ip , bool bypass_robot);
        void connect();
        void activate();
        void home();
        void deactivate();
        void disconnect();
        void reset_error();
        void print_number(double number);
        double get_velocity();
        void move_lin_vel_trf(double velocity);
        void set_conf(short c1, short c2, short c3);
        void move_pose(double x, double y, double z, double alpha, double beta, double gamma);
};

#endif