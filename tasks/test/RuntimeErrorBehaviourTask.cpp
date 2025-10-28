/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "RuntimeErrorBehaviourTask.hpp"
#include "../ConfigureGuard.hpp"

using namespace iodrivers_base;
using namespace iodrivers_base::test;
using namespace std;

static const int DUMMY_BUFFER_SIZE = 4;

namespace {
    class DummyDriver : public iodrivers_base::Driver {
    public:
        DummyDriver()
            : iodrivers_base::Driver(4)
        {
        }

        int extractPacket(boost::uint8_t const* buffer, size_t size) const
        {
            if (size >= 4) {
                return 4;
            }
            return 0;
        }
    };
}

RuntimeErrorBehaviourTask::RuntimeErrorBehaviourTask(std::string const& name)
    : RuntimeErrorBehaviourTaskBase(name)
{
}

RuntimeErrorBehaviourTask::~RuntimeErrorBehaviourTask()
{
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See RuntimeErrorBehaviourTask.hpp for more detailed
// documentation about them.

bool RuntimeErrorBehaviourTask::configureHook()
{
    if (!RuntimeErrorBehaviourTaskBase::configureHook())
        return false;

    setRuntimeErrorIOProcessingEnabled(_runtime_error_io_processing_enabled.get());
    return true;
}
bool RuntimeErrorBehaviourTask::startHook()
{
    if (!RuntimeErrorBehaviourTaskBase::startHook())
        return false;
    return true;
}
void RuntimeErrorBehaviourTask::updateHook()
{
    RuntimeErrorBehaviourTaskBase::updateHook();

    error(CUSTOM_ERROR);
}
void RuntimeErrorBehaviourTask::errorProcessIO()
{
    uint8_t buffer[DUMMY_BUFFER_SIZE];
    int packet_size = mDriver->readPacket(buffer, DUMMY_BUFFER_SIZE);

    RawPacket packet;
    packet.time = base::Time::now();
    packet.data = std::vector<uint8_t>(buffer, buffer + packet_size);
    _rx.write(packet);
}

void RuntimeErrorBehaviourTask::errorHook()
{
    RuntimeErrorBehaviourTaskBase::errorHook();
}
void RuntimeErrorBehaviourTask::stopHook()
{
    RuntimeErrorBehaviourTaskBase::stopHook();
}
void RuntimeErrorBehaviourTask::cleanupHook()
{
    RuntimeErrorBehaviourTaskBase::cleanupHook();
}
