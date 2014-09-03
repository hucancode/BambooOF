local input_file = "Metrics.xml";
local output_file = "verdana.xml";
local fin = io.open(input_file, "r")
local content = fin:read("*all")
content = content:gsub(">\n    <x>"," x =\"")
content = content:gsub("</x>\n    <y>","\" y =\"")
content = content:gsub("</y>\n    <width>","\" width =\"")
content = content:gsub("</width>\n    <height>","\" height =\"")
content = content:gsub("</height>\n  </character>","\" />")
fin:close()
local fout = io.open(output_file, "w")
fout:write(content)
fout:close()