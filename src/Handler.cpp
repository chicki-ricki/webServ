/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wcollen <wcollen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 15:24:49 by mlarra            #+#    #+#             */
/*   Updated: 2022/12/07 16:57:15 by wcollen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Handler.hpp"

Handler::Handler(std::vector<Server> * enterServers): _servers(enterServers)
{
	// _servers = enterServers;
}

Handler::~Handler()
{
}

void	Handler::initFds()
{
	FD_ZERO(&_fdSet);
	// FD_ZERO(&_fdReadSave);
	// FD_ZERO(&_fdWriteSave);
	// FD_ZERO(&_fdRead);
	// FD_ZERO(&_fdWrite);
	_maxFd = 0;
	for (int i = 0; i < _servers->size(); i++)
	{
		(*_servers)[i].initSocket();
		FD_SET((*_servers)[i].getSocketFd(), &_fdSet);
		// FD_SET((*_servers)[i].getSocketFd(), &_fdReadSave);
		if ((*_servers)[i].getSocketFd() > _maxFd)
			_maxFd = (*_servers)[i].getSocketFd();
	}
	if (_maxFd == 0)
		exitError("Could not init Fds in Handler");
}

void	Handler::serverRun()
{
	while (true)
	{
		fd_set			fdRead;
		fd_set			fdWrite;
		int				ret;
		// int				serverCount;
		// struct timeval	timeout;

		// while (ret == 0)
		// {
			// timeout.tv_sec = 1;
			// timeout.tv_usec = 0;
			// memcpy(&fdRead, &_fdReadSave, sizeof(_fdReadSave));
			// memcpy(&fdRead, &_fdSet, sizeof(_fdSet));
			// FD_ZERO(&fdWrite);
			// for (std::vector<int>::iterator it = _fds.begin(); it != _fds.end(); it++)
			// {
			// 	FD_SET(*it, &fdWrite);
			// }
			// ret = select(_maxFd + 1, &fdRead, &fdWrite, 0, 0);
		// }
		memcpy(&fdRead, &_fdSet, sizeof(_fdSet));
		memcpy(&fdWrite, &_fdSet, sizeof(_fdSet));
		ret = select(_maxFd + 1, &fdRead, &fdWrite, 0, 0);
		if (ret == -1)
			exitError("Select");
		//проход по серверам
		for (int i = 0; i < _servers->size(); i++)
		{
			int	serverFd = (*_servers)[i].getSocketFd();

			if (FD_ISSET(serverFd, &fdRead))
			{
				Client client = Client(serverFd);
				client.acceptClient();
				FD_SET(client.getFd(), &_fdSet);
				_clients.push_back(client);
				// serverCount = i;
				if (_maxFd < i)
					_maxFd = i;
			}
		}
		//проход по читающим fd
		for (std::vector<Client>::iterator it = _clients.begin(); ret && it != _clients.end(); it++)
		{
			int		fdClient = it->getFd();
			char	buffer[RECV_SIZE] = {0};
			// int		ret;

			if (FD_ISSET(fdClient, &fdRead))
			{
				ret = recv(fdClient, buffer, RECV_SIZE - 1, 0);
				if (ret > 0)
				{
					buffer[ret] = 0;
					it->request += buffer;
				}
//where is a process chank?!
				// else if (ret == 0)
				// {}
				else if (ret == -1)
				{
					FD_CLR(fdClient, &_fdSet);
					FD_CLR(fdClient, &fdRead);
					// delete *it;
					_clients.erase(it);
					it = _clients.begin();
				}
				ret = 0;
				break;
			}
		}
	}
}