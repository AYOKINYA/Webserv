#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "Utils.hpp"

class Config
{
	private:
			char        *_conf_path;
			std::string _str;
			typedef std::map<std::string, std::string> elem;
			typedef std::map<std::string, elem> conf;
			conf _conf;
 
	public:
			Config(char *conf);
			Config(const Config& copy);
			Config& operator=(const Config& other);
			virtual ~Config();

			void	init(fd_set *rset, fd_set *wset, fd_set *cp_rset, fd_set *cp_wset);
			
			void	readfile(char *path);
			void	parse();
			void	get_conf(std::string line, std::string &context);
			void	set_location(std::string &line, std::string &context);
			void	fill_conf(void);

			class	ConfigException : public std::exception
			{
				private:
					std::string	function;
					std::string	error;
				public:
					ConfigException();
					ConfigException(std::string function, std::string error);
					virtual ~ConfigException() throw();
					virtual const char	*what() const throw();
			};
};

#endif