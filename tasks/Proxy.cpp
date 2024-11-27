/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Proxy.hpp"
#include <iodrivers_base/Driver.hpp>

using namespace iodrivers_base;

namespace
{
    class DummyDriver : public Driver
    {
    public:
        DummyDriver(int size)
            : Driver(size) {}

        int extractPacket(boost::uint8_t const* buffer, size_t size) const
        {
            return size;
        }
    };
}

Proxy::Proxy(std::string const& name)
    : ProxyBase(name)
{
}

Proxy::Proxy(std::string const& name, RTT::ExecutionEngine* engine)
    : ProxyBase(name, engine)
{
}

Proxy::~Proxy()
{
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Proxy.hpp for more detailed
// documentation about them.

bool Proxy::configureHook()
{
    m_buffer_size = createProxyDriver();
    m_rx_packet.data.reserve(m_buffer_size);
    m_tx_packet.data.reserve(m_buffer_size);
    m_packet_buffer.resize(m_buffer_size);

    if (!_io_port.get().empty())
        mDriver->openURI(_io_port.get());

    if (! ProxyBase::configureHook())
        return false;

    return true;
}

int Proxy::createProxyDriver()
{
    int size = _internal_buffer_size.get();
    setDriver(new DummyDriver(size));
    return size;
}

void Proxy::writePacket(RawPacket const& packet)
{
    mDriver->writePacket(m_tx_packet.data.data(), m_tx_packet.data.size());
}

void Proxy::readPacket(RawPacket& packet)
{
    int packet_size = mDriver->readPacket(m_packet_buffer.data(), m_packet_buffer.size());
    packet.time = base::Time::now();
    packet.data = std::vector<uint8_t>(
        m_packet_buffer.begin(), m_packet_buffer.begin() + packet_size
    );
}

bool Proxy::startHook()
{
    if (! ProxyBase::startHook())
        return false;
    return true;
}
void Proxy::updateHook()
{
    ProxyBase::updateHook();

    while (_tx.read(m_tx_packet, false) == RTT::NewData)
        writePacket(m_tx_packet);
}
void Proxy::processIO()
{
    try
    {
        readPacket(m_rx_packet);
        _rx.write(m_rx_packet);
    }
    catch (TimeoutError const&)
    {
    }
}

void Proxy::errorHook()
{
    ProxyBase::errorHook();
}
void Proxy::stopHook()
{
    ProxyBase::stopHook();
}
void Proxy::cleanupHook()
{
    ProxyBase::cleanupHook();
    delete mDriver;
    mDriver = 0;
}
