#include "hsv/Selector.hpp"

namespace hsv
{
    Selector::Selector()
        : m_sockCount(0)
    {
        FD_ZERO(&master);
        FD_ZERO(&copy);
    }
    // Clear socket set
    void Selector::Clear()
    {
        FD_ZERO(&master);
    }

    bool Selector::isReady(const Socket& sock)
    {
        return FD_ISSET(sock.GetHandle(), &master) == 0;
    }

    // Remove from socket set
    void Selector::Remove(Socket& sock)
    {
        auto handle = sock.GetHandle();
        if (!FD_ISSET(handle, &master)) {
            return;
        }
        --m_sockCount;
        FD_CLR(handle, &master);
    }

    // Add socket at socket set
    void Selector::Add(Socket& sock)
    {
        if (m_sockCount >= FD_SETSIZE) {
            return;
        }
        auto handle = sock.GetHandle();
        if (FD_ISSET(handle, &master))
        {
            return;
        }
        FD_SET(handle, &master);
    }
    int Selector::Wait(std::chrono::milliseconds time)
    {
        timeval tv;
        tv.tv_sec = time.count() / 10000;
        tv.tv_usec = time.count() % 10000;

        copy = master;

        int count = select(0, &copy, nullptr, nullptr, time.count() != 0 ? &tv : nullptr);

        return count;
    }
    Socket Selector::GetReadySocket(int num)
    {
        SOCKET s = copy.fd_array[num];
        Socket sock;
        sock.SetHandle(s);

        return sock;
    }
}