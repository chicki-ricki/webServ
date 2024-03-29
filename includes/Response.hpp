/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlarra <mlarra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 15:25:53 by mlarra            #+#    #+#             */
/*   Updated: 2023/02/09 12:36:22 by mlarra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include "webserv.hpp"

typedef  std::map<std::string, std::string> strStrMap;

class Server;
class ResponseConfig;
class Request;

class Response
{
private:
	std::string					_response;
	std::string					_path;
	int							_code;
	std::string					_type;
	bool						_isAutoIndex;
	struct s_listen				*_hostPort;
	std::map<int, std::string>	_errorMap; // ??????????
	// std::string					_boundary;
	// std::string					_endBoundary;
	// std::string					_fullBuffer;
	// bool						_endBody;

	static std::map<std::string, void (Response::*)(Request &, ResponseConfig &)>	_method;
	static std::map<std::string, void (Response::*)(Request &, ResponseConfig &)>	initMetods();

public:
	Response();
	~Response();

	void		call(Request &request, ResponseConfig &responseConf);
	int			readContent(Request &request);
	std::string	readHtml(const std::string &);
	void		methodGet(Request &, ResponseConfig &);
	void		methodPost(Request &, ResponseConfig &);
	void		methodDelete(Request &, ResponseConfig &);
	std::string	getResponse();
	void		requestBodyParsing(Request & request);

};

#endif