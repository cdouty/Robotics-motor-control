#include "TransportSubsystem.h"

#include <frc/smartdashboard/SmartDashboard.h>

TransportSubsystem::TransportSubsystem(
        int enableButtonIndex,
        rev::CANSparkMax& TransportDrive,
        frc::Joystick& stick
) :
    m_runTransport(false),
    m_buttonIndex(enableButtonIndex),
    m_transportDrive{TransportDrive},
    m_stick{stick}
{
  m_transportDrive.RestoreFactoryDefaults();
}


void TransportSubsystem::ModeInit()
{
    // Call GetRawButtonPressed to discard any button presses
    // made while the robot was disabled.
    m_stick.GetRawButtonPressed(m_buttonIndex);
    StopMotor();
}

// This is mostly copied from SPARK-MAX-Examples/C++/Get and Set Parameters
void TransportSubsystem::RobotInit()
{
    /**
     * The RestoreFactoryDefaults method can be used to reset the configuration parameters
     * in the SPARK MAX to their factory default state. If no argument is passed, these
     * parameters will not persist between power cycles
     */
    m_transportDrive.RestoreFactoryDefaults();

    /**
     * Parameters can be set by calling the appropriate Set method on the CANSparkMax object
     * whose properties you want to change
     * 
     * Set methods will return one of three REVLibError values which will let you know if the 
     * parameter was successfully set:
     *  REVLibError::kOk
     *  REVLibError::kError
     *  REVLibError::kTimeout
     */
    if(m_transportDrive.SetIdleMode(rev::CANSparkMax::IdleMode::kCoast) != rev::REVLibError::kOk) {
      frc::SmartDashboard::PutString("Transport Idle Mode", "Error");
    }

    /**
     * Similarly, parameters will have a Get method which allows you to retrieve their values
     * from the controller
     */
    if(m_transportDrive.GetIdleMode() == rev::CANSparkMax::IdleMode::kCoast) {
      frc::SmartDashboard::PutString("Transport Idle Mode", "Coast");
    } else {
      frc::SmartDashboard::PutString("Transport Idle Mode", "Brake");
    }

    // Set ramp rate to 0
    if(m_transportDrive.SetOpenLoopRampRate(0) != rev::REVLibError::kOk) {
      frc::SmartDashboard::PutString("Transport Ramp Rate", "Error");
    }

    // read back ramp rate value
    frc::SmartDashboard::PutNumber("Transport Ramp Rate", m_transportDrive.GetOpenLoopRampRate());

    // Display local member values.
    frc::SmartDashboard::PutBoolean("Run Transport", m_runTransport);
}


bool TransportSubsystem::RunPeriodic()
{
    // Toggle Transport state on button press.
    if (m_stick.GetRawButtonPressed(m_buttonIndex))
    {
      m_runTransport = !m_runTransport;
    }
    // 
    if (m_runTransport)
    {
    // Throttle is connected the slider on the controller.
    // The throttle axis reads -1.0 when pressed forward.
      m_transportDrive.Set(-m_stick.GetThrottle());
    } else {
      m_transportDrive.Set(0);
    }
    // periodically read voltage, temperature, and applied output and publish to SmartDashboard
    frc::SmartDashboard::PutNumber("Transport Voltage", m_transportDrive.GetBusVoltage());
    frc::SmartDashboard::PutNumber("Transport Temperature", m_transportDrive.GetMotorTemperature());
    frc::SmartDashboard::PutNumber("Transport Output", m_transportDrive.GetAppliedOutput());
    frc::SmartDashboard::PutBoolean("Run Transport", m_runTransport);
    return m_runTransport;
}


void TransportSubsystem::StopMotor()
{
    m_runTransport = false;
    m_transportDrive.StopMotor();
}
