#include "Response.hpp"

Response::Response()
{
	init_status_table();
};

Response::Response(const Response &copy)
{
	*this = copy;
}

Response& Response::operator=(const Response &copy)
{
	if (this == &copy)
		return (*this);

	_header = copy._header;
	_start_line = copy._start_line;

	return (*this);
}

Response::~Response()
{};

void Response::clear(void)
{
	_header.clear();
	_start_line.clear();
	_body.clear();
}

void Response::init_status_table(void)
{
	_status_table.insert(std::make_pair(200, "OK"));
	_status_table.insert(std::make_pair(201, "CREATED"));
	_status_table.insert(std::make_pair(204, "No Content"));
	_status_table.insert(std::make_pair(400, "Bad Request"));
	_status_table.insert(std::make_pair(401, "Unauthorized"));
	_status_table.insert(std::make_pair(404, "Not Found"));
	_status_table.insert(std::make_pair(405, "Not Allowed Method"));
	_status_table.insert(std::make_pair(411, "Length Required"));
	_status_table.insert(std::make_pair(413, "Request Entity Too Large"));
	_status_table.insert(std::make_pair(415, "Unsupported Media Type"));
	_status_table.insert(std::make_pair(501, "Not Implemented"));
	_status_table.insert(std::make_pair(505, "HTTP Version Not Supported"));
}

