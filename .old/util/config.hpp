/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: config.hpp                                                    $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/config.hpp 7     8/07/04 5:50p Ian                 $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file  config.hpp
 *  \brief Configuration
 *
 *  Configuration class. Loads/Saves data from/to a file or stream,
 *  interfaces with memory variables in bulk;
 *  organizes entries as [group]tag=value.
 */

#ifndef UTIL_CONFIG_HPP_INCLUDED
#define UTIL_CONFIG_HPP_INCLUDED

  #include "../types/types.hpp"
  #include <fstream>
  #include <string>
  #include <map>

  namespace util {

//-----------------------------------------------------------------------------

  /// Configuration
  class Config
  {
    NO_GEN(Config)

  public:
    #define EXC_IDS EMPTY_FILENAME,CANT_OPEN_FILE,LOAD_FAILED
    DECLARE_EXC_ROOT
    #undef EXC_IDS

  private:
    struct lessIgnoreCase: std::binary_function<std::string,std::string,bool> 
    {
	  bool operator()(std::string const& a, std::string const& b) const
      {
          return stricmp(a.c_str(),b.c_str()) < 0;
      }
	};
      
    /// mapping tag -> value
    typedef std::map<std::string const,std::string,lessIgnoreCase> tag_map;
    /// mapping group -> (tag->value)
    typedef std::map<std::string const,tag_map,lessIgnoreCase>     grp_map;
    /// data
    grp_map m_cfgData;

    class not_found ///< exception, used internally
    {
    };

    /// find a [group, tag] string
    std::string FindString(pcstr grp, pcstr tag) const throw(not_found);

  public:
    /// construct an empty object
    Config();

    /// load data from a stream
    void Load(std::istream&)        throw(exc);
    /// save data to a stream
    void Save(std::ostream&)  const throw(exc);

    /// load data from a file
    void Load(pcstr fileName)       throw(exc);
    /// svae data to a file
    void Save(pcstr fileName) const throw(exc);

    /// get a string value, return defValue if not found
    std::string GetString(pcstr grp, pcstr tag, std::string const& defValue) const;
    /// get a string value, return defValue if not found
    std::string GetString(pcstr grp, pcstr tag, pcstr defValue)              const;

    /// get a double value, return defValue if not found
    double      GetDouble(pcstr grp, pcstr tag, double defValue)             const;
    /// get an integer value, return defValue if not found
    int         GetInt   (pcstr grp, pcstr tag, int    defValue)             const;
    /// get an unsigned integer value, return defValue if not found
    uint        GetUint  (pcstr grp, pcstr tag, uint   defValue)             const;
    /// get a boolean value, return defValue if not found
    bool        GetBool  (pcstr grp, pcstr tag, bool   defValue)             const;

    /// remove all data
    void Clear();
    /// remove all data for a group
    void Erase(pcstr grp);
    /// remove a specific piece of data
    void Erase(pcstr grp, pcstr tag);

    /// set a string value
    void SetString(pcstr grp, pcstr tag, std::string const& value);
    /// set a string value
    void SetString(pcstr grp, pcstr tag, pcstr value);

    /// set a double value
    void SetDouble(pcstr grp, pcstr tag, double value);
    /// set an integer value
    void SetInt   (pcstr grp, pcstr tag, int    value);
    /// set an unsigned integer value
    void SetUint  (pcstr grp, pcstr tag, uint   value);
    /// set a boolean value
    void SetBool  (pcstr grp, pcstr tag, bool   value);

  public:
    /// memory interface: the pointer to a variable
    union config_var
    {
      std::string *s;
      double      *d;
      int         *i;
      uint        *u;
      bool        *b;
    };

    /// memory interface; default value
    union config_def
    {
      pcstr        s;
      double       d;
      int          i;
      uint         u;
      bool         b;
    };

    /// memory interface; one entry
    struct config_dat
    {
      config_dat(pcstr grp, pcstr tag, std::string& var, pcstr  def);
      config_dat(pcstr grp, pcstr tag, double&      var, double def);
      config_dat(pcstr grp, pcstr tag, int&         var, int    def);
      config_dat(pcstr grp, pcstr tag, uint&        var, uint   def);
      config_dat(pcstr grp, pcstr tag, bool&        var, bool   def);
      config_dat();

      config_dat(config_dat const&);

      pcstr       grp;
      pcstr       tag;
      char        type;
      config_var  var;
      config_def  def;
    };

    /// load data from memory locations (bulk set of variables)
    void Load(config_dat const[]);
    /// save data to memory locations
    void Save(config_dat const[]) const;
  };

//-----------------------------------------------------------------------------

  } // namespace

#endif

//** eof **********************************************************************
