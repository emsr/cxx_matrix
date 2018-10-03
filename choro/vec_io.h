#ifndef VEC_IO_H
#define VEC_IO_H 1

#include <sstream>

template<typename _CharT, typename _Traits = std::char_traits<_CharT>,
	 typename _Num, std::size_t _Dim>
  std::basic_ostream<_CharT, _Traits>&
  operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	     const space_point<_Num, _Dim>& pt)
  {
    std::basic_ostringstream<_CharT, _Traits> __s;
    __s.flags(__os.flags());
    __s.imbue(__os.getloc());
    __s.precision(__os.precision());
    for (unsigned int c = 0; c < _Dim; ++c)
      __s << (c == 0 ? '{' : ',') << pt[c];
    __s << '}';
    return __os << __s.str();
  }

template<typename _CharT, typename _Traits = std::char_traits<_CharT>,
	 typename _Num, std::size_t _Dim>
  std::basic_ostream<_CharT, _Traits>&
  operator<<(std::basic_ostream<_CharT, _Traits>& __os,
	     const space_vector<_Num, _Dim>& vec)
  {
    std::basic_ostringstream<_CharT, _Traits> __s;
    __s.flags(__os.flags());
    __s.imbue(__os.getloc());
    __s.precision(__os.precision());
    for (unsigned int c = 0; c < _Dim; ++c)
      __s << (c == 0 ? '{' : ',') << vec[c];
    __s << '}';
    return __os << __s.str();
  }

#endif // VEC_IO_H
