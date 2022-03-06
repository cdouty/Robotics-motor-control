#pragma once

#include <frc/Joystick.h>

#include "rev/CANSparkMax.h"

class LaunchSubsystem {
public:
    LaunchSubsystem(
        int enableButtonIndex,
        rev::CANSparkMax& launchDrive,
        frc::Joystick& stick
    );

    // Initialize the subsystem from Robot::RobotInit().
    void RobotInit();
    // Call this when entering any mode.
    void ModeInit();
    // Call this in Periodic() function to check button and set motor.
    bool RunPeriodic();

    // Stop the motor and disable run state.
    void StopMotor();

public:
    // These are for tests an inspection.
    bool isEnabled() const {return m_runLaunch;}
    int buttonIndex() const {return m_buttonIndex;}
    bool SetEnable(bool value)
    {
        m_runLaunch = value;
        return isEnabled();
    }

private:
    bool m_runLaunch;
    const int m_buttonIndex;
    // Non-owning reference to the motor controller.
    rev::CANSparkMax& m_LaunchDrive;
    // Non-owning reference to the joystick.
    frc::Joystick& m_stick;
    // Encoder object created to display velocity values
    //rev::SparkMaxRelativeEncoder m_Launchencoder = m_LaunchDrive.GetEncoder();
};