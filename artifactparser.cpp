#include "artifactparser.h"

void GetXmlText(rapidxml::xml_node<>* curnode, std::string* contents)
{
    if(curnode->type() == rapidxml::node_data || curnode->type() == rapidxml::node_cdata || curnode->type() == rapidxml::node_comment || curnode->type() == rapidxml::node_doctype)
    {
        contents->append(curnode->value());
        contents->append("\n");
    }
    rapidxml::xml_node<>* curchild = curnode->first_node();
    while(curchild != NULL)
    {
        GetXmlText(curchild, contents);
        curchild = curchild->next_sibling();
    }
}

void ParseArtifact(ForImg* curforimg, FileItem* curfileitem, bool* inmemory, uint8_t* tmpbuf, FILE* tmpfile, std::string* filecontents)
{
    std::string tmpfilestr = "/tmp/wf/" + curfileitem->name + "-" + std::to_string(curfileitem->gid) + ".tmp";
    if(curfileitem->sig.compare("Microsoft Word 2007+") == 0) // word document
    {
        zip_error_t err;
        int interr = 0;
        int zipfileid = 0;
        std::string zipfilename = "";
        zip_uint64_t zipfilesize = 0;
        zip_t* curzip = NULL;
        if(inmemory)
        {
            zip_source_t* zipsrc = zip_source_buffer_create(tmpbuf, curfileitem->size, 1, &err);
            curzip = zip_open_from_source(zipsrc, ZIP_RDONLY, &err);
        }
        else
        {
            curzip = zip_open(tmpfilestr.c_str(), ZIP_RDONLY, &interr);
        }
        int64_t zipentrycnt = zip_get_num_entries(curzip, 0);
        //std::cout << "zip entries count: " << zipentrycnt << std::endl;
        struct zip_stat zipstat;
        zip_stat_init(&zipstat);
        for(int i=0; i < zipentrycnt; i++)
        {
            zip_stat_index(curzip, i, 0, &zipstat);
            if(strcmp(zipstat.name, "word/document.xml") == 0)
            {
                zipfileid = i;
                zipfilename = zipstat.name;
                zipfilesize = zipstat.size;
            }
        }
        //std::cout << "zipfileid: " << zipfileid << std::endl;
        zip_file_t* docxml = zip_fopen_index(curzip, zipfileid, ZIP_FL_UNCHANGED);
        char zipfilebuf[zipfilesize+1];
        zip_int64_t bytesread = zip_fread(docxml, zipfilebuf, zipfilesize);
        zipfilebuf[zipfilesize] = 0;
        interr = zip_fclose(docxml);

        rapidxml::xml_document<> worddoc;
        worddoc.parse<0>(zipfilebuf);
        rapidxml::xml_node<>* rootnode = worddoc.first_node();
        GetXmlText(rootnode, filecontents);
        //std::cout << filecontents << std::endl;
    }
    else
        std::cout << "launch internal/external viewer for files here..." << std::endl;
}
