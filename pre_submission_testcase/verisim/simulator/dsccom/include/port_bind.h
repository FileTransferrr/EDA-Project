#ifndef PORT_BIND_H
#define PORT_BIND_H

#include <list>
#include "systemc.h"
#include "vpi_user.h"

namespace dsccom
{
    typedef void (*SignalSetter4s)(void* signal_ptr, sc_dt::sc_logic* value);
    typedef void (*SignalSetterInt)(void* signal_ptr, int value);
    typedef void (*SignalSetterReal)(void* signal_ptr, double value);
    typedef void (*SignalSetterStr)(void* signal_ptr, const char* value);
    typedef void (*DestroySignal)(void* signal_ptr);
    // Return if the signal->change_stamp() is in [first_stamp, last_stamp]
    typedef bool (*SignalReader)(void* signal_ptr, t_vpi_value& value, sc_dt::uint64 first_stamp, sc_dt::uint64 last_stamp);

    struct ScPortInfo
    {
        vpiHandle vpi_port;
        void* signal;
        SignalSetter4s setter_4s;
        SignalSetterInt setter_int;
        SignalSetterReal setter_real;
        SignalSetterStr setter_str;
        SignalReader reader;
    private:
        DestroySignal destructor;
    public:

        ScPortInfo(const ScPortInfo&) = delete;
        ScPortInfo(ScPortInfo&& other)
        {
            signal = other.signal;
            other.signal = nullptr;
            setter_4s = other.setter_4s;
            setter_int = other.setter_int;
            setter_real = other.setter_real;
            setter_str = other.setter_str;
            destructor = other.destructor;
            reader = other.reader;
        }
        ScPortInfo(vpiHandle vpi, void* sig, SignalSetter4s set,
            SignalSetterInt set_int, SignalSetterReal set_real,
            SignalSetterStr set_str,
            SignalReader read, DestroySignal destruct)
            : vpi_port(vpi), signal(sig), setter_4s(set), setter_int(set_int), setter_real(set_real),
            setter_str(set_str), reader(read), destructor(destruct)
        {}

        virtual ~ScPortInfo()
        {
            assert(destructor!= nullptr);
            if (destructor != nullptr && signal != nullptr)
                destructor(signal);
        }
    };

typedef std::list<ScPortInfo> ScPortInfoKeeper;
}

// pointers to bind Vpi and SC ports.
// exist only during sc_port->report_signal_type method call
extern vpiHandle current_vpi_port;
extern dsccom::ScPortInfoKeeper* current_ports_keeper;

namespace dsccom
{
inline ScPortInfo* bindVpiScPorts(ScPortInfoKeeper& keeper, vpiHandle port, sc_port_base* c_port)
{
    current_vpi_port = port;
    const auto old_ports_count = keeper.size();
    current_ports_keeper = &keeper;
    c_port->report_signal_type(std::cout);
    current_vpi_port = nullptr;
    current_ports_keeper = nullptr;

    if (keeper.size() > old_ports_count)
        return &keeper.back();

    return nullptr;
}
}

#endif //PORT_BIND_H