
#include "precomp.h"
#include "ident_server.h"
#include "ident_server_connection.h"

IdentServer::IdentServer()
{
}

IdentServer::~IdentServer()
{
	stop();
}

void IdentServer::start()
{
	std::unique_lock<std::mutex> lock(mutex);
	if (!stop_flag)
		return;

	stop_flag = false;
	lock.unlock();
	change_event.notify();

	thread = std::thread(&IdentServer::worker_main, this);
}

void IdentServer::stop()
{
	std::unique_lock<std::mutex> lock(mutex);
	stop_flag = true;
	lock.unlock();
	change_event.notify();
	thread.join();
}

void IdentServer::worker_main()
{
	std::vector<std::shared_ptr<IdentServerConnection>> connections;

	try
	{
		auto listen = uicore::TCPListen::listen(uicore::SocketName("113"));
		std::unique_lock<std::mutex> lock(mutex);
		while (!stop_flag)
		{
			uicore::SocketName end_point;
			uicore::TCPConnectionPtr connection = listen->accept(end_point);
			if (connection)
			{
				connections.push_back(std::make_shared<IdentServerConnection>(this, connection));
			}
			else
			{
				uicore::NetworkEvent *events[] = { listen.get() };
				change_event.wait(lock, 1, events);
			}
		}
	}
	catch (uicore::Exception &)
	{
	}
}
