/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlarra <mlarra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 16:25:58 by mlarra            #+#    #+#             */
/*   Updated: 2023/01/25 15:09:36 by mlarra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include "webserv.hpp"

class Request;
class ResponseConfig;

class CgiHandler
{
private:
	std::map<std::string, std::string>	_env;
	std::string							_body;
	void								initEnv(Request &, ResponseConfig &);
	char								**convertEnvToCStrArr() const;
public:
	CgiHandler(Request &, ResponseConfig &);
	virtual ~CgiHandler();
	std::string	executeCgi(const std::string &);
};

#endif