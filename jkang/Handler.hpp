#ifndef HANDLER_HPP
# define HANDLER_HPP
# include "Request.hpp"
# include "Response.hpp"
# include "Client.hpp"
# include "Utils.hpp"


class Handler
{
    public:
            void    exec_method(Client &client);
			
            void	setDate(Client& client);
			void	setContentType(Client& client);
			void	setContentLocation(Client& client);
			void	setContentLanguage(Client &client);
			void	setContentLength(const std::string &content);
			void	setAllow(Client& client);
			void	setLastModified(Client& client);
			void	setServer(Client &client);
			void	setTransferEncoding(Client &client);
			void	setWWWAuthentication(Client &client);

			std::string printItem(const std::string &key);
			std::string printItem2(std::map<std::string, std::string> param, const std::string &key);
			std::string getStartLine(Client &client);
			std::string header(const std::string &path);
			std::string body(const std::string &path);
			std::string exec_method(void);

			void    Get(Client &client);
			void    Head(Client &client);
			void    Post(Client &client);
			void    Put(Client &client);
			void    Delete(Client &client);
			void    Options(Client &client);
            void    Connect(Client &client);
			void    Trace(Client &client);

			void	cgi(std::string extension, Client &client);
            void	parseCGIResult(Client &client);
			char **		Env(Client &client);
			std::string	autoindex(Client &client);
            void    create_response(Client& client);
			

};

#endif