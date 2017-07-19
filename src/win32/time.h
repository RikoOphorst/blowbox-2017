#pragma once

namespace blowbox
{
    /** @brief Manages the time in Blowbox. */
    class Time
    {
    public:
        Time();
        ~Time();

        /** @brief Indicate to the Time instance that a new frame has begun.*/
        void NewFrame();

        /** @returns The delta time as a float. */
        static float GetDeltaTime();
        /** @returns The delta time as a double. */
        static double GetDeltaTimeAsDouble();

        /** @returns The process time as a float. */
        static float GetProcessTime();
        /** @returns The process time as a double. */
        static double GetProcessTimeAsDouble();

    private:
        double initialization_time_;    //!< The time at which the Time instance was created.
        double delta_time_;             //!< The current delta time.
        double frame_start_time_;       //!< The current frame's start time.
        double process_time_;           //!< The time the process has been alive for.
    };
}