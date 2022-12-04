/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlarra <mlarra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 12:25:05 by mlarra            #+#    #+#             */
/*   Updated: 2022/12/02 10:55:09 by mlarra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

void	exitError(std::string str)
{
	std::cerr << str << std::endl;
	exit(1);
}



int main(int ac, char **av)
{
	int					socketFd;
	int					maxFd;
	fd_set				fdReadMaster;
	fd_set				fdRead;
	fd_set				fdWriteMaster;
	fd_set				fdWrite;
	int					rezSelect;
	struct sockaddr_in	addrIn;
	int					fd;
	int					addrLen;
	long				valread;

	(void) av;
	maxFd = 0;
	if (ac != 2)
		exitError("Wrong numbers of arguments");
	addrIn.sin_family = AF_INET;
	addrIn.sin_port = htons(8080);
	addrIn.sin_addr.s_addr = INADDR_ANY;
	memset(addrIn.sin_zero, '\0', sizeof(addrIn.sin_zero));
	struct sockaddr *addr = reinterpret_cast<struct sockaddr *>(&addrIn);
	addrLen = sizeof(*addr);
	socketFd = socket(addrIn.sin_family, SOCK_STREAM, 0);
//Для связывания сокета с адресом и номером порта используют системный вызов bind
	if (socketFd == -1)// || bind(socketFd, addr, sizeof(*addr)) == -1 ||\
		// listen(socketFd, 32)  == -1)
		exitError("Socket create error");
	if (bind(socketFd, addr, sizeof(*addr)) == -1)
		exitError("Bind error");
	if (listen(socketFd, 32) == -1)
		exitError("Listen error");

	if (socketFd >= maxFd)
		maxFd = socketFd + 1;
	FD_ZERO(&fdReadMaster);
	FD_SET(socketFd, &fdReadMaster);
	FD_ZERO(&fdWriteMaster);
	FD_SET(socketFd, &fdWriteMaster);
	char *hello = "Hello world!";
	// char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	while (1)
	{
		std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
		fdRead = fdReadMaster;
		fdWrite = fdWriteMaster;
		rezSelect = select(maxFd, &fdRead, &fdWrite, 0, 0);
		if (rezSelect == -1)
			exitError("Select error");
		else if (rezSelect)
			std::cout << "Данные доступны." << std::endl;
		else
			std::cout << "Данные не появились в течение пяти секунд." << std::endl;
		fd = accept(socketFd, addr, (socklen_t *)&addrLen);
		if (fd < 0)
		{
			exitError("In accept");
		}
		char buffer[30000] = {0};
		valread = recv(fd, buffer, 30000, MSG_PEEK);
		printf("%s\n", buffer);
		// send(fd, hello, strlen(hello), 0);
		write(fd, hello, strlen(hello));
		std::cout << "------------------Hello message sent-------------------\n" << std::endl;
		close(fd);
	}
	close(socketFd);
	return (0);
}