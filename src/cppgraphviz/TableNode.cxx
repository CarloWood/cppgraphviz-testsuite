#include "sys.h"
#include "TableNode.hpp"
#include <iostream>

namespace cppgraphviz {

void TableNodeData::write_html_to(std::ostream& os, std::string const& indentation) const
{
  bool table_has_bgcolor = attribute_list().has_key("bgcolor");
  bool table_has_color = attribute_list().has_key("color");
  bool table_has_fontname = attribute_list().has_key("fontname");
  bool table_has_fontsize = attribute_list().has_key("fontsize");
  bool table_has_fontcolor = attribute_list().has_key("fontcolor");
  bool table_has_font = table_has_fontname || table_has_fontsize || table_has_fontcolor;

  os << indentation << dot_id() << " [shape=none, margin=0.1, label=<\n" <<
        indentation << "  <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\"";
  if (table_has_color)
    os << " COLOR=\"" << attribute_list().get_value("color") << '"';
  os << ">\n" <<
        indentation << "    <TR><TD BORDER=\"0\"></TD></TR>\n";
  for (int port = 0; port < elements_.size(); ++port)
  {
    AttributeList const& eal = elements_[port].attribute_list();
    os << indentation << "    <TR><TD PORT=\"" << port << "\"";
    if (eal.has_key("bgcolor"))
      os << " BGCOLOR=\"" << eal.get_value("bgcolor") << '"';
    else if (table_has_bgcolor)
      os << " BGCOLOR=\"" << attribute_list().get_value("bgcolor") << '"';
    if (eal.has_key("color"))
      os << " COLOR=\"" << eal.get_value("color") << '"';
    os << '>';
    bool has_fontname = eal.has_key("fontname");
    bool has_fontsize = eal.has_key("fontsize");
    bool has_fontcolor = eal.has_key("fontcolor");
    bool has_font = table_has_font || has_fontname || has_fontsize || has_fontcolor;
    if (has_font)
    {
      os << "<FONT";
      if (has_fontname)
        os << " FACE=\"" << eal.get_value("fontname") << "\"";
      else if (table_has_fontname)
        os << " FACE=\"" << attribute_list().get_value("fontname") << "\"";
      if (has_fontsize)
        os << " POINT-SIZE=\"" << eal.get_value("fontsize") << "\"";
      else if (table_has_fontsize)
        os << " POINT-SIZE=\"" << attribute_list().get_value("fontsize") << "\"";
      if (has_fontcolor)
        os << " COLOR=\"" << eal.get_value("fontcolor") << "\"";
      else if (table_has_fontcolor)
        os << " COLOR=\"" << attribute_list().get_value("fontcolor") << "\"";
      os << '>';
    }
    os << elements_[port].label();
    if (has_font)
      os << "</FONT>";
    os << "</TD></TR>\n";
  }
  os << indentation << "  </TABLE>\n" <<
        indentation << ">];\n";
}

} // namespace cppgraphviz