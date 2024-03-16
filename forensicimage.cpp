#include "forensicimage.h"

ForImg::ForImg(std::string imgfile)
{
    // SET IMGPATH - PATH TO THE FORENSIC IMAGE
    imgpath = imgfile;
    int rfound = imgfile.rfind(".");
    libewf_error_t* ewferr = NULL;
    libvhdi_error_t* vhderr = NULL;
    libqcow_error_t* qcowerr = NULL;
    libvmdk_error_t* vmdkerr = NULL;
    libphdi_error_t* phderr = NULL;
    // SET IMGTYPE - DETERMINES HOW READ THE RAW CONTENT
    std::string imgext = imgfile.substr(rfound+1);
    if(imgext.compare("dd") == 0 || imgext.compare("DD") == 0) // RAW
        imgtype = 1;
    else if(libewf_check_file_signature(imgfile.c_str(), &ewferr) == 1 || imgext.compare("e01") == 0 || imgext.compare("E01") == 0) // EWF
        imgtype = 2;
    else if(imgext.compare("aff4") == 0 || imgext.compare("AFF4") == 0) // AFF4
        imgtype = 3;
    else if(imgext.compare("000") == 0 || imgext.compare("00") == 0 || imgext.compare("001") == 0) // SPLIT RAW
        imgtype = 4;
    else if(imgext.compare("wfi") == 0) // WFI
        imgtype = 5;
    else if(imgext.compare("wli") == 0) // WLI
        imgtype = 6;
    else if(libvhdi_check_file_signature(imgfile.c_str(), &vhderr) == 1 || imgext.compare("vhd") == 0 || imgext.compare("vhdx") == 0 || imgext.compare("VHD") == 0 || imgext.compare("VHDX") == 0) // VHD/VHDX
        imgtype = 7;
    else if(libqcow_check_file_signature(imgfile.c_str(), &qcowerr) == 1 || imgext.compare("qcow") == 0 || imgext.compare("qcow2") == 0 || imgext.compare("QCOW") == 0 || imgext.compare("QCOW2") == 0) // QCOW/QCOW2
	imgtype = 8;
    else if(libvmdk_check_file_signature(imgfile.c_str(), &vmdkerr) == 1 || imgext.compare("vmdk") == 0 || imgext.compare("VMDK") == 0) // VMDK
	imgtype = 9;
    else if(libphdi_check_file_signature(imgfile.c_str(), &phderr) == 1 || imgext.compare("phd") == 0 || imgext.compare("PHD") == 0) // PHD
	imgtype = 10;
    else if(imgext.compare("sfs") == 0 || imgext.compare("squashfs") == 0) // SFS
	imgtype = 11;
    else // ANY OLD FILE
        imgtype = 0;
    libewf_error_free(&ewferr);
    libvhdi_error_free(&vhderr);
    libqcow_error_free(&qcowerr);
    libvmdk_error_free(&vmdkerr);
    libphdi_error_free(&phderr);
    // SET IMGSIZE - GET THE SIZE OF THE RAW CONTENT
    if(imgtype == 1) // RAW
    {
        imagebuffer.open(imgfile.c_str(), std::ios::in|std::ios::binary);
        imagebuffer.seekg(0, imagebuffer.beg);
        imagebuffer.seekg(0, imagebuffer.end);
        imgsize = imagebuffer.tellg();
        imagebuffer.close();
        //std::cout << imgfile << " size: " << imgsize << std::endl;
    }
    else if(imgtype == 2) // EWF
    {
        libewf_handle_t* ewfhandle = NULL;
        libewf_error_t* ewferror = NULL;
        char** globfiles = NULL;
        int globfilecnt = 0;
	int found = imgpath.rfind(".E");
	if(found == -1)
	    found = imgpath.rfind(".e");
	std::string imgprematch = imgpath.substr(0, found+2);
	std::filesystem::path imagepath(imgpath);
	imagepath.remove_filename();
	//std::cout << "Image Path: " << imgpath << std::endl;
	//std::cout << "Image Pre Match: " << imgprematch << std::endl;
	//std::cout << "imagepath: " << imagepath.string() << std::endl;
	for(const auto &file : std::filesystem::directory_iterator(imagepath))
	{
	    if(file.path().string().compare(0, found+2, imgprematch) == 0)
		globfilecnt++;
		//std::cout << "match: " << file.path().string() <<std::endl;
	}
	char* filenames[globfilecnt] = {NULL};
	filenames[0] = (char*)imgpath.c_str();
	int i = 1;
	for(const auto &file : std::filesystem::directory_iterator(imagepath))
	{
	    if(file.path().string().compare(0, found+2, imgprematch) == 0)
	    {
		if(file.path().string().compare(imgpath) != 0)
		{
		    filenames[i] = (char*)file.path().string().c_str();
		    i++;
		}
	    }
	}
        int retopen = 0;
        retopen = libewf_glob(filenames[0], strlen(filenames[0]), LIBEWF_FORMAT_UNKNOWN, &globfiles, &globfilecnt, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        retopen = libewf_handle_initialize(&ewfhandle, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        retopen = libewf_handle_open(ewfhandle, globfiles, globfilecnt, LIBEWF_OPEN_READ, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        libewf_handle_get_media_size(ewfhandle, &imgsize, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        libewf_handle_close(ewfhandle, &ewferror);
        libewf_handle_free(&ewfhandle, &ewferror);
        libewf_glob_free(globfiles, globfilecnt, &ewferror);
	libewf_error_free(&ewferror);
        //std::cout << imgfile << " size: " << imgsize << std::endl;
    }
    else if(imgtype == 3) // AFF4
    {
        AFF4_init();
        int aff4handle = AFF4_open(imgpath.c_str());
        imgsize = AFF4_object_size(aff4handle);
        AFF4_close(aff4handle);
	//std::cout << imgfile << " size: " << imgsize << std::endl;
    }
    else if(imgtype == 4) // SPLIT RAW
    {
	libsmraw_handle_t* smhandle = NULL;
	libsmraw_error_t* smerror = NULL;
	char** globfiles = NULL;
	int globfilecnt = 0;
	int found = imgpath.rfind(".0");
	if(found == -1)
	    found = imgpath.rfind(".a");
	std::string imgprematch = imgpath.substr(0, found+2);
	std::filesystem::path imagepath(imgpath);
	imagepath.remove_filename();
	for(const auto &file : std::filesystem::directory_iterator(imagepath))
	{
	    if(file.path().string().compare(0, found+2, imgprematch) == 0)
		globfilecnt++;
	}
	char* filenames[globfilecnt] = {NULL};
	filenames[0] = (char*)imgpath.c_str();
	int i = 1;
	for(const auto &file : std::filesystem::directory_iterator(imagepath))
	{
	    if(file.path().string().compare(0, found+2, imgprematch) == 0)
	    {
		if(file.path().string().compare(imgpath) != 0)
		{
		    filenames[i] = (char*)file.path().string().c_str();
		    i++;
		}
	    }
	}
	int retopen = 0;
	retopen = libsmraw_glob(filenames[0], strlen(filenames[0]), &globfiles, &globfilecnt, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
	retopen = libsmraw_handle_initialize(&smhandle, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
	retopen = libsmraw_handle_open(smhandle, globfiles, globfilecnt, LIBSMRAW_OPEN_READ, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
	libsmraw_handle_get_media_size(smhandle, &imgsize, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
	libsmraw_handle_close(smhandle, &smerror);
	libsmraw_handle_free(&smhandle, &smerror);
	libsmraw_glob_free(globfiles, globfilecnt, &smerror);
	libsmraw_error_free(&smerror);
        //std::cout << imgfile << " size: " << imgsize << std::endl;
    }
    else if(imgtype == 5) // WFI
    {
	std::filesystem::path tmppath = std::filesystem::canonical(imgpath);
	std::string imgfilename = tmppath.filename().string();
	int efound = imgfilename.rfind(".");
	std::string vdirname = "/" + imgfilename.substr(0, efound) + "/" + imgfilename.substr(0, efound) + ".dd";

	Filesystem filesystem;
	WltgReader pack_wltg(imgpath.c_str());
	filesystem.add_source(&pack_wltg);
	std::unique_ptr<BaseFileStream> handle = filesystem.open_file_read(vdirname.c_str());
	if(!handle)
	{
	    std::cout << "failed to open file" << std::endl;
	}
	imgsize = handle->size();
    }
    else if(imgtype == 6) // WLI
    {
    }
    else if(imgtype == 7) // VHD/VHDX
    {
        libvhdi_error_t* vhderror = NULL;
        libvhdi_file_t* vhdfile = NULL;
        int retopen = 0;
        retopen = libvhdi_file_initialize(&vhdfile, &vhderror);
        if(retopen == -1)
            libvhdi_error_fprint(vhderror, stdout);
        retopen = libvhdi_file_open(vhdfile, imgpath.c_str(), LIBVHDI_OPEN_READ, &vhderror);
        if(retopen == -1)
            libvhdi_error_fprint(vhderror, stdout);
        libvhdi_file_get_media_size(vhdfile, &imgsize, &vhderror);
        if(retopen == -1)
            libvhdi_error_fprint(vhderror, stdout);
        libvhdi_file_close(vhdfile, &vhderror);
        libvhdi_file_free(&vhdfile, &vhderror);
        libvhdi_error_free(&vhderror);
    }
    else if(imgtype == 8) // QCOW/QCOW2
    {
	libqcow_error_t* qcowerror = NULL;
	libqcow_file_t* qcowfile = NULL;
	int ret = 0;
	ret = libqcow_file_initialize(&qcowfile, &qcowerror);
	if(ret == -1)
	    libqcow_error_fprint(qcowerror, stdout);
	ret = libqcow_file_open(qcowfile, imgpath.c_str(), LIBQCOW_OPEN_READ, &qcowerror);
	if(ret == -1)
	    libqcow_error_fprint(qcowerror, stdout);
	ret = libqcow_file_get_media_size(qcowfile, &imgsize, &qcowerror);
	if(ret == -1)
	    libqcow_error_fprint(qcowerror, stdout);
	libqcow_file_close(qcowfile, &qcowerror);
	libqcow_file_free(&qcowfile, &qcowerror);
	libqcow_error_free(&qcowerror);
    }
    else if(imgtype == 9) // VMDK
    {
	libvmdk_error_t* vmdkerror = NULL;
	libvmdk_handle_t* vmdkfile = NULL;
	int ret = 0;
	ret = libvmdk_handle_initialize(&vmdkfile, &vmdkerror);
	if(ret == -1)
	    libvmdk_error_fprint(vmdkerror, stdout);
	ret = libvmdk_handle_open(vmdkfile, imgpath.c_str(), LIBVMDK_OPEN_READ, &vmdkerror);
	if(ret == -1)
	    libvmdk_error_fprint(vmdkerror, stdout);
	ret = libvmdk_handle_get_media_size(vmdkfile, &imgsize, &vmdkerror);
	if(ret == -1)
	    libvmdk_error_fprint(vmdkerror, stdout);
	libvmdk_handle_close(vmdkfile, &vmdkerror);
	libvmdk_handle_free(&vmdkfile, &vmdkerror);
	libvmdk_error_free(&vmdkerror);
    }
    else if(imgtype == 10) // PHD
    {
	libphdi_error_t* phdierror = NULL;
	libphdi_handle_t* phdifile = NULL;
	int ret = 0;
	ret = libphdi_handle_initialize(&phdierror, &phdierror);
	if(ret == -1)
	    libphdi_error_fprint(phdierror, stdout);
	ret = libphdi_handle_open(phdifile, imgpath.c_str(), LIBPHDI_OPEN_READ, &phdierror);
	if(ret == -1)
	    libphdi_error_fprint(phdierror, stdout);
	ret = libphdi_handle_get_media_size(phdifile, &imgsize, &phdierror);
	if(ret == -1)
	    libphdi_error_fprint(phdierror, stdout);
	libphdi_handle_close(phdifile, &phdierror);
	libphdi_handle_free(&phdifile, &phdierror);
	libphdi_error_free(&phdierror);
    }
    else if(imgtype == 11) // SFS
    {
	sqfs_file_t* sqfsfile = NULL;
	sqfs_compressor_t* sqfscmp = NULL;
	sqfs_xattr_reader_t* sqfsxattr = NULL;
	sqfs_data_reader_t* sqfsdata = NULL;
	sqfs_dir_reader_t* sqfsdirrd = NULL;
	sqfs_id_table_t* sqfsidtbl = NULL;
	sqfs_tree_node_t* sqfsnode = NULL;
	sqfs_u8* p = NULL;
	sqfs_u8* output = NULL;
	sqfs_compressor_config_t sqfscmpcfg;
	sqfs_super_t sqfssuper;
	sqfs_u64 sqfsfilesize;

	sqfsfile = sqfs_open_file(imgpath.c_str(), SQFS_FILE_OPEN_READ_ONLY);
	if(sqfsfile == NULL)
	    std::cout << "squash file system file open failed." << std::endl;
	int ret = sqfs_super_read(&sqfssuper, sqfsfile);
	if(ret)
	    std::cout << "error opening super block." << std::endl;
	sqfs_compressor_config_init(&sqfscmpcfg, (SQFS_COMPRESSOR)sqfssuper.compression_id, sqfssuper.block_size, SQFS_COMP_FLAG_UNCOMPRESS);
	ret = sqfs_compressor_create(&sqfscmpcfg, &sqfscmp);
	if(ret != 0)
	    std::cout << "compressor creation failed " << ret << std::endl;
	if(!(sqfssuper.flags & SQFS_FLAG_NO_XATTRS))
	{
	    sqfsxattr = sqfs_xattr_reader_create(0);
	    if(sqfsxattr == NULL)
		std::cout << "error creating xattr reader: " << SQFS_ERROR_ALLOC << std::endl;
	    ret = sqfs_xattr_reader_load(sqfsxattr, &sqfssuper, sqfsfile, sqfscmp);
	    if(ret)
		std::cout << "error loading xattr reader " << ret << std::endl;
	}
	sqfsidtbl = sqfs_id_table_create(0);
	if(sqfsidtbl == NULL)
	    std::cout << "error creating id table: " << SQFS_ERROR_ALLOC << std::endl;
	ret = sqfs_id_table_read(sqfsidtbl, sqfsfile, &sqfssuper, sqfscmp);
	if(ret)
	    std::cout << "error reading id table: " << ret << std::endl;
	sqfsdirrd = sqfs_dir_reader_create(&sqfssuper, sqfscmp, sqfsfile, 0);
	if(sqfsdirrd == NULL)
	    std::cout << "error creating dir reader: " << SQFS_ERROR_ALLOC << std::endl;
	sqfsdata = sqfs_data_reader_create(sqfsfile, sqfssuper.block_size, sqfscmp, 0);
	if(sqfsdata == NULL)
	    std::cout << "error creating data reader: " << SQFS_ERROR_ALLOC << std::endl;
	ret = sqfs_data_reader_load_fragment_table(sqfsdata, &sqfssuper);
	if(ret)
	    std::cout << "error loading fragment table: " << ret << std::endl;
	std::string virtpath = "/image.raw";
	ret = sqfs_dir_reader_get_full_hierarchy(sqfsdirrd, sqfsidtbl, "/image.raw", 0, &sqfsnode);
	if(ret)
	    std::cout << "error reading filesystem hierarchy: " << ret << std::endl;
	if(!S_ISREG(sqfsnode->inode->base.mode))
	    std::cout << "/image.raw is not a file" << std::endl;
	sqfs_inode_get_file_size(sqfsnode->inode, &sqfsfilesize);
	imgsize = sqfsfilesize;

	sqfs_dir_tree_destroy(sqfsnode);
	sqfs_destroy(sqfsdata);
	sqfs_destroy(sqfsdirrd);
	sqfs_destroy(sqfsidtbl);
	sqfs_destroy(sqfsxattr);
	sqfs_destroy(sqfscmp);
	sqfs_destroy(sqfsfile);
    }
    else // everything else
    {
	std::filesystem::path imagepath(imgpath);
        imgsize = std::filesystem::file_size(imgpath);
	//std::cout << imgfile << " size: " << std::filesystem::file_size(imgpath) << std::endl;
    }
}

void ForImg::ReadContent(uint8_t* buf, uint64_t pos, uint64_t size)
{
    if(imgtype == 1) // RAW
    {
        imagebuffer.open(imgpath.c_str(), std::ios::in|std::ios::binary);
        imagebuffer.seekg(pos);
        imagebuffer.read((char*)buf, size);
        imagebuffer.close();
    }
    else if(imgtype == 2) // EWF
    {
        libewf_handle_t* ewfhandle = NULL;
        libewf_error_t* ewferror = NULL;
        char** globfiles = NULL;
        int globfilecnt = 0;
        int found = imgpath.rfind(".E");
        if(found == -1)
            found = imgpath.rfind(".e");
        std::string imgprematch = imgpath.substr(0, found+2);
        std::filesystem::path imagepath(imgpath);
        imagepath.remove_filename();
        for(const auto &file : std::filesystem::directory_iterator(imagepath))
        {
            if(file.path().string().compare(0, found+2, imgprematch) == 0)
                globfilecnt++;
        }
        char* filenames[globfilecnt] = {NULL};
        filenames[0] = (char*)imgpath.c_str();
        int i = 1;
        for(const auto &file : std::filesystem::directory_iterator(imagepath))
        {
            if(file.path().string().compare(0, found+2, imgprematch) == 0)
            {
                if(file.path().string().compare(imgpath) != 0)
                {
                    filenames[i] = (char*)file.path().string().c_str();
                    i++;
                }
            }
        }
        int retopen = 0;
        retopen = libewf_glob(filenames[0], strlen(filenames[0]), LIBEWF_FORMAT_UNKNOWN, &globfiles, &globfilecnt, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        retopen = libewf_handle_initialize(&ewfhandle, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        retopen = libewf_handle_open(ewfhandle, globfiles, globfilecnt, LIBEWF_OPEN_READ, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        uint64_t res = 0;
        imgoffset = libewf_handle_seek_offset(ewfhandle, pos, SEEK_SET, &ewferror);
        res = libewf_handle_read_buffer(ewfhandle, buf, size, &ewferror);
        libewf_handle_close(ewfhandle, &ewferror);
        libewf_handle_free(&ewfhandle, &ewferror);
        libewf_glob_free(globfiles, globfilecnt, &ewferror);
        libewf_error_free(&ewferror);
    }
    else if(imgtype == 3) // AFF4
    {
        AFF4_init();
        int aff4handle = AFF4_open(imgpath.c_str());
        int bytesread = AFF4_read(aff4handle, pos, buf, size);
        AFF4_close(aff4handle);
    }
    else if(imgtype == 4) // SPLIT RAW
    {
	libsmraw_handle_t* smhandle = NULL;
	libsmraw_error_t* smerror = NULL;
	char** globfiles = NULL;
	int globfilecnt = 0;
	int found = imgpath.rfind(".0");
	if(found == -1)
	    found = imgpath.rfind(".a");
	std::string imgprematch = imgpath.substr(0, found+2);
	std::filesystem::path imagepath(imgpath);
	imagepath.remove_filename();
	for(const auto &file : std::filesystem::directory_iterator(imagepath))
	{
	    if(file.path().string().compare(0, found+2, imgprematch) == 0)
		globfilecnt++;
	}
	char* filenames[globfilecnt] = {NULL};
	filenames[0] = (char*)imgpath.c_str();
	int i = 1;
	for(const auto &file : std::filesystem::directory_iterator(imagepath))
	{
	    if(file.path().string().compare(0, found+2, imgprematch) == 0)
	    {
		if(file.path().string().compare(imgpath) != 0)
		{
		    filenames[i] = (char*)file.path().string().c_str();
		    i++;
		}
	    }
	}
	int retopen = 0;
	retopen = libsmraw_glob(filenames[0], strlen(filenames[0]), &globfiles, &globfilecnt, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
	retopen = libsmraw_handle_initialize(&smhandle, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
	retopen = libsmraw_handle_open(smhandle, globfiles, globfilecnt, LIBSMRAW_OPEN_READ, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
        uint64_t res = 0;
        imgoffset = libsmraw_handle_seek_offset(smhandle, pos, SEEK_SET, &smerror);
        res = libsmraw_handle_read_buffer(smhandle, buf, size, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
	libsmraw_handle_close(smhandle, &smerror);
	libsmraw_handle_free(&smhandle, &smerror);
	libsmraw_glob_free(globfiles, globfilecnt, &smerror);
	libsmraw_error_free(&smerror);
    }
    else if(imgtype == 5) // WFI
    {
	std::filesystem::path tmppath = std::filesystem::canonical(imgpath);
	std::string imgfilename = tmppath.filename().string();
	int efound = imgfilename.rfind(".");
	std::string vdirname = "/" + imgfilename.substr(0, efound) + "/" + imgfilename.substr(0, efound) + ".dd";

	Filesystem filesystem;
	WltgReader pack_wltg(imgpath.c_str());
	filesystem.add_source(&pack_wltg);
	std::unique_ptr<BaseFileStream> handle = filesystem.open_file_read(vdirname.c_str());
	if(!handle)
	{
	    std::cout << "failed to open file" << std::endl;
	}
	handle->seek(pos);
	uint64_t bytesread = handle->read_into(buf, size);
    }
    else if(imgtype == 6) // WLI
    {
    }
    else if(imgtype == 7) // VHD/VHDX
    {
        libvhdi_error_t* vhderror = NULL;
        libvhdi_file_t* vhdfile = NULL;
        int retopen = 0;
        retopen = libvhdi_file_initialize(&vhdfile, &vhderror);
        if(retopen == -1)
            libvhdi_error_fprint(vhderror, stdout);
        retopen = libvhdi_file_open(vhdfile, imgpath.c_str(), LIBVHDI_OPEN_READ, &vhderror);
        if(retopen == -1)
            libvhdi_error_fprint(vhderror, stdout);
        uint64_t res = 0;
        imgoffset = libvhdi_file_seek_offset(vhdfile, pos, SEEK_SET, &vhderror);
        res = libvhdi_file_read_buffer(vhdfile, buf, size, &vhderror);
        if(retopen == -1)
            libvhdi_error_fprint(vhderror, stdout);
        libvhdi_file_close(vhdfile, &vhderror);
        libvhdi_file_free(&vhdfile, &vhderror);
        libvhdi_error_free(&vhderror);
    }
    else if(imgtype == 8) // QCOW/QCOW2
    {
	libqcow_error_t* qcowerror = NULL;
	libqcow_file_t* qcowfile = NULL;
	int ret = 0;
	ret = libqcow_file_initialize(&qcowfile, &qcowerror);
	if(ret == -1)
	    libqcow_error_fprint(qcowerror, stdout);
	ret = libqcow_file_open(qcowfile, imgpath.c_str(), LIBQCOW_OPEN_READ, &qcowerror);
	if(ret == -1)
	    libqcow_error_fprint(qcowerror, stdout);
	uint64_t res = 0;
	imgoffset = libqcow_file_seek_offset(qcowfile, pos, SEEK_SET, &qcowerror);
	res = libqcow_file_read_buffer(qcowfile, buf, size, &qcowerror);
	if(ret == -1)
	    libqcow_error_fprint(qcowerror, stdout);
	libqcow_file_close(qcowfile, &qcowerror);
	libqcow_file_free(&qcowfile, &qcowerror);
	libqcow_error_free(&qcowerror);
    }
    else if(imgtype == 9) // VMDK
    {
	libvmdk_error_t* vmdkerror = NULL;
	libvmdk_handle_t* vmdkfile = NULL;
	int ret = 0;
	ret = libvmdk_handle_initialize(&vmdkfile, &vmdkerror);
	if(ret == -1)
	    libvmdk_error_fprint(vmdkerror, stdout);
	ret = libvmdk_handle_open(vmdkfile, imgpath.c_str(), LIBVMDK_OPEN_READ, &vmdkerror);
	if(ret == -1)
	    libvmdk_error_fprint(vmdkerror, stdout);
	uint64_t res = 0;
	imgoffset = libvmdk_handle_seek_offset(vmdkfile, pos, SEEK_SET, &vmdkerror);
	res = libvmdk_handle_read_buffer(vmdkfile, buf, size, &vmdkerror);
	if(ret == -1)
	    libvmdk_error_fprint(vmdkerror, stdout);
	libvmdk_handle_close(vmdkfile, &vmdkerror);
	libvmdk_handle_free(&vmdkfile, &vmdkerror);
	libvmdk_error_free(&vmdkerror);
    }
    else if(imgtype == 10) // PHDI
    {
	libphdi_error_t* phdierror = NULL;
	libphdi_handle_t* phdifile = NULL;
	int ret = 0;
	ret = libphdi_handle_initialize(&phdifile, &phdierror);
	if(ret == -1)
	    libphdi_error_fprint(phdierror, stdout);
	ret = libphdi_handle_open(phdifile, imgpath.c_str(), LIBPHDI_OPEN_READ, &phdierror);
	if(ret == -1)
	    libphdi_error_fprint(phdierror, stdout);
	uint64_t res = 0;
	imgoffset = libphdi_handle_seek_offset(phdifile, pos, SEEK_SET, &phdierror);
	res = libphdi_handle_read_buffer(phdifile, buf, size, &phdierror);
	if(ret == -1)
	    libphdi_error_fprint(phdierror, stdout);
	libphdi_handle_close(phdifile, &phdierror);
	libphdi_handle_free(&phdifile, &phdierror);
	libphdi_error_free(&phdierror);
    }
    else if(imgtype == 11) // SFS
    {
	sqfs_file_t* sqfsfile = NULL;
	sqfs_compressor_t* sqfscmp = NULL;
	sqfs_xattr_reader_t* sqfsxattr = NULL;
	sqfs_data_reader_t* sqfsdata = NULL;
	sqfs_dir_reader_t* sqfsdirrd = NULL;
	sqfs_id_table_t* sqfsidtbl = NULL;
	sqfs_tree_node_t* sqfsnode = NULL;
	sqfs_u8* p = NULL;
	sqfs_u8* output = NULL;
	sqfs_compressor_config_t sqfscmpcfg;
	sqfs_super_t sqfssuper;
	sqfs_u64 sqfsfilesize;

	sqfsfile = sqfs_open_file(imgpath.c_str(), SQFS_FILE_OPEN_READ_ONLY);
	if(sqfsfile == NULL)
	    std::cout << "squash file system file open failed." << std::endl;
	int ret = sqfs_super_read(&sqfssuper, sqfsfile);
	if(ret)
	    std::cout << "error opening super block." << std::endl;
	sqfs_compressor_config_init(&sqfscmpcfg, (SQFS_COMPRESSOR)sqfssuper.compression_id, sqfssuper.block_size, SQFS_COMP_FLAG_UNCOMPRESS);
	ret = sqfs_compressor_create(&sqfscmpcfg, &sqfscmp);
	if(ret != 0)
	    std::cout << "compressor creation failed " << ret << std::endl;
	if(!(sqfssuper.flags & SQFS_FLAG_NO_XATTRS))
	{
	    sqfsxattr = sqfs_xattr_reader_create(0);
	    if(sqfsxattr == NULL)
		std::cout << "error creating xattr reader: " << SQFS_ERROR_ALLOC << std::endl;
	    ret = sqfs_xattr_reader_load(sqfsxattr, &sqfssuper, sqfsfile, sqfscmp);
	    if(ret)
		std::cout << "error loading xattr reader " << ret << std::endl;
	}
	sqfsidtbl = sqfs_id_table_create(0);
	if(sqfsidtbl == NULL)
	    std::cout << "error creating id table: " << SQFS_ERROR_ALLOC << std::endl;
	ret = sqfs_id_table_read(sqfsidtbl, sqfsfile, &sqfssuper, sqfscmp);
	if(ret)
	    std::cout << "error reading id table: " << ret << std::endl;
	sqfsdirrd = sqfs_dir_reader_create(&sqfssuper, sqfscmp, sqfsfile, 0);
	if(sqfsdirrd == NULL)
	    std::cout << "error creating dir reader: " << SQFS_ERROR_ALLOC << std::endl;
	sqfsdata = sqfs_data_reader_create(sqfsfile, sqfssuper.block_size, sqfscmp, 0);
	if(sqfsdata == NULL)
	    std::cout << "error creating data reader: " << SQFS_ERROR_ALLOC << std::endl;
	ret = sqfs_data_reader_load_fragment_table(sqfsdata, &sqfssuper);
	if(ret)
	    std::cout << "error loading fragment table: " << ret << std::endl;
	std::string virtpath = "/image.raw";
	ret = sqfs_dir_reader_get_full_hierarchy(sqfsdirrd, sqfsidtbl, "/image.raw", 0, &sqfsnode);
	if(ret)
	    std::cout << "error reading filesystem hierarchy: " << ret << std::endl;
	if(!S_ISREG(sqfsnode->inode->base.mode))
	    std::cout << "/image.raw is not a file" << std::endl;
	sqfs_inode_get_file_size(sqfsnode->inode, &sqfsfilesize);
	/*
	output = p = malloc(file_size);
	if (output == NULL) {
		fprintf(stderr, "malloc failed: %d\n", errno);
		goto out;
	}

	for (size_t i = 0; i < sqfs_inode_get_file_block_count(n->inode); ++i) {
		size_t chunk_size, read = (file_size < super.block_size) ? file_size : super.block_size;
		sqfs_u8 *chunk;

		ret = sqfs_data_reader_get_block(data, n->inode, i, &chunk_size, &chunk);
		if (ret) {
			fprintf(stderr, "reading data block: %d\n", ret);
			goto out;
		}

		memcpy(p, chunk, chunk_size);
		p += chunk_size;
		free(chunk);

		file_size -= read;
	}

	if (file_size > 0) {
		size_t chunk_size;
		sqfs_u8 *chunk;

		ret = sqfs_data_reader_get_fragment(data, n->inode, &chunk_size, &chunk);
		if (ret) {
			fprintf(stderr, "reading fragment block: %d\n", ret);
			goto out;
		}

		memcpy(p, chunk, chunk_size);
		free(chunk);
	}

	// for example simplicity, assume this is a text file
	fprintf(stdout, "%s\n", (char *)output);
	*/ 
	sqfs_dir_tree_destroy(sqfsnode);
	sqfs_destroy(sqfsdata);
	sqfs_destroy(sqfsdirrd);
	sqfs_destroy(sqfsidtbl);
	sqfs_destroy(sqfsxattr);
	sqfs_destroy(sqfscmp);
	sqfs_destroy(sqfsfile);
	/*
    	// lazy man method using SQUASHFUSE COMMAND LINE TOOL
	std::string sqfusepath = std::string("squashfuse " + imgpath + " " + "/tmp/sfsmnt/");
	std::string squfusepath = std::string("fusermount -u /tmp/sfsmnt/");

	// MOUNT SQUASH FS IMAGE
	std::system(sqfusepath.c_str());

	imagebuffer.open("/tmp/sfsmnt/image.raw", std::ios::in|std::ios::binary);
        imagebuffer.seekg(pos);
        imagebuffer.read((char*)buf, size);
        imagebuffer.close();

	// UNMOUNT SQUASH FS IMAGE
	std::system(squfusepath.c_str());
	*/
    }
    else // EVERYTHING ELSE
    {
        imagebuffer.open(imgpath.c_str(), std::ios::in|std::ios::binary);
        imagebuffer.seekg(pos);
        imagebuffer.read((char*)buf, size);
        imagebuffer.close();
    }
}

uint64_t ForImg::Size()
{
    return imgsize;
}

std::string ForImg::SizeString()
{
    std::stringstream ss;
    ss.imbue(std::locale(""));
    ss << std::fixed << imgsize;
    return ss.str();
}

int8_t ForImg::ImgType()
{
    return imgtype;
}

std::string ForImg::ImageFullPath()
{
    return imgpath;
}

std::string ForImg::ImagePath()
{
    return std::filesystem::path(imgpath).remove_filename();
}

std::string ForImg::ImageFileName()
{
    return std::filesystem::path(imgpath).filename();
}

/*
QString ForImg::MountPath()
{
    return mntpath;
}

void ForImg::SetMountPath(QString mountpath)
{
    mntpath = mountpath;
    mntpath.chop(1);
}
*/

void GetForImgProperties(std::string* imgpath, std::string* propstr)
{
    libewf_error_t* ewferror = NULL;
    libvhdi_error_t* vhderror = NULL;
    libqcow_error_t* qcowerror = NULL;
    libvmdk_error_t* vmdkerror = NULL;
    int rfound = imgpath->rfind(".");
    std::string imgext = imgpath->substr(rfound+1);
    if(libewf_check_file_signature(imgpath->c_str(), &ewferror) == 1 || imgext.compare("e01") == 0 || imgext.compare("E01") == 0) // EWF
    {
        libewf_handle_t* ewfhandle = NULL;
        char** globfiles = NULL;
        int globfilecnt = 0;
	int found = imgpath->rfind(".E");
	if(found == -1)
	    found = imgpath->rfind(".e");
	std::string imgprematch = imgpath->substr(0, found+2);
	std::filesystem::path imagepath(*imgpath);
	imagepath.remove_filename();
	for(const auto &file : std::filesystem::directory_iterator(imagepath))
	{
	    if(file.path().string().compare(0, found+2, imgprematch) == 0)
		globfilecnt++;
	}
	char* filenames[globfilecnt] = {NULL};
	filenames[0] = (char*)imgpath->c_str();
	int i = 1;
	for(const auto &file : std::filesystem::directory_iterator(imagepath))
	{
	    if(file.path().string().compare(0, found+2, imgprematch) == 0)
	    {
		if(file.path().string().compare(*imgpath) != 0)
		{
		    filenames[i] = (char*)file.path().string().c_str();
		    i++;
		}
	    }
	}
        int retopen = 0;
        retopen = libewf_glob(filenames[0], strlen(filenames[0]), LIBEWF_FORMAT_UNKNOWN, &globfiles, &globfilecnt, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        retopen = libewf_handle_initialize(&ewfhandle, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        retopen = libewf_handle_open(ewfhandle, globfiles, globfilecnt, LIBEWF_OPEN_READ, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
	/* GET EWF PROPERTIES */
	// CASE NUMBER
	size_t casenumbersize = 0;
	retopen = libewf_handle_get_utf8_header_value_size(ewfhandle, (uint8_t*)"case_number", 11, &casenumbersize, &ewferror);
	uint8_t* casenumbervalue = new uint8_t[casenumbersize];
	retopen = libewf_handle_get_utf8_header_value(ewfhandle, (uint8_t*)"case_number", 11, casenumbervalue, casenumbersize, &ewferror);
	std::string casenumber = (char*)casenumbervalue;
	// EXAMINER NAME
	size_t examinernamesize = 0;
	retopen = libewf_handle_get_utf8_header_value_size(ewfhandle, (uint8_t*)"examiner_name", 13, &examinernamesize, &ewferror);
	uint8_t* examinernamevalue = new uint8_t[examinernamesize];
	retopen = libewf_handle_get_utf8_header_value(ewfhandle, (uint8_t*)"examiner_name", 13, examinernamevalue, examinernamesize, &ewferror);
	std::string examinername = (char*)examinernamevalue;
	// DESCRIPTION
	size_t descriptionsize = 0;
	retopen = libewf_handle_get_utf8_header_value_size(ewfhandle, (uint8_t*)"description", 11, &descriptionsize, &ewferror);
	uint8_t* descriptionvalue = new uint8_t[descriptionsize];
	retopen = libewf_handle_get_utf8_header_value(ewfhandle, (uint8_t*)"description", 11, descriptionvalue, descriptionsize, &ewferror);
	std::string description = (char*)descriptionvalue;
	// EVIDENCE NUMBER
	size_t evidencenumbersize = 0;
	retopen = libewf_handle_get_utf8_header_value_size(ewfhandle, (uint8_t*)"evidence_number", 15, &evidencenumbersize, &ewferror);
	uint8_t* evidencenumbervalue = new uint8_t[evidencenumbersize];
	retopen = libewf_handle_get_utf8_header_value(ewfhandle, (uint8_t*)"evidence_number", 15, evidencenumbervalue, evidencenumbersize, &ewferror);
	std::string evidencenumber = (char*)evidencenumbervalue;
	// NOTES
	size_t notessize = 0;
	retopen = libewf_handle_get_utf8_header_value_size(ewfhandle, (uint8_t*)"notes", 5, &notessize, &ewferror);
	uint8_t* notesvalue = new uint8_t[notessize];
	retopen = libewf_handle_get_utf8_header_value(ewfhandle, (uint8_t*)"notes", 5, notesvalue, notessize, &ewferror);
	std::string notes = (char*)notesvalue;
	// ACQUIRY DATE
	size_t acquirydatesize = 0;
	retopen = libewf_handle_get_utf8_header_value_size(ewfhandle, (uint8_t*)"acquiry_date", 12, &acquirydatesize, &ewferror);
	uint8_t* acquirydatevalue = new uint8_t[acquirydatesize];
	retopen = libewf_handle_get_utf8_header_value(ewfhandle, (uint8_t*)"acquiry_date", 12, acquirydatevalue, acquirydatesize, &ewferror);
	std::string acquirydate = (char*)acquirydatevalue;
	// SYSTEM DATE
	size_t systemdatesize = 0;
	retopen = libewf_handle_get_utf8_header_value_size(ewfhandle, (uint8_t*)"system_date", 11, &systemdatesize, &ewferror);
	uint8_t* systemdatevalue = new uint8_t[systemdatesize];
	retopen = libewf_handle_get_utf8_header_value(ewfhandle, (uint8_t*)"system_date", 11, systemdatevalue, systemdatesize, &ewferror);
	std::string systemdate = (char*)systemdatevalue;
	// ACQUISITION OPERATING SYSTEM
	size_t acquiryoperatingsystemsize = 0;
	retopen = libewf_handle_get_utf8_header_value_size(ewfhandle, (uint8_t*)"acquiry_operating_system", 24, &acquiryoperatingsystemsize, &ewferror);
	uint8_t* acquiryopsysvalue = new uint8_t[acquiryoperatingsystemsize];
	retopen = libewf_handle_get_utf8_header_value(ewfhandle, (uint8_t*)"acquiry_operating_system", 24, acquiryopsysvalue, acquiryoperatingsystemsize, &ewferror);
	std::string acquiryoperatingsystem = (char*)acquiryopsysvalue;
	// ACQUISITION SOFTWARE VERSION
	size_t acquirysoftwareversionsize = 0;
	retopen = libewf_handle_get_utf8_header_value_size(ewfhandle, (uint8_t*)"acquiry_software_version", 24, &acquirysoftwareversionsize, &ewferror);
	uint8_t* acquirysoftvervalue = new uint8_t[acquirysoftwareversionsize];
	retopen = libewf_handle_get_utf8_header_value(ewfhandle, (uint8_t*)"acquiry_software_version", 24, acquirysoftvervalue, acquirysoftwareversionsize, &ewferror);
	std::string acquirysoftwareversion = (char*)acquirysoftvervalue;
	// PASSWORD
	size_t passwordsize = 0;
	retopen = libewf_handle_get_utf8_header_value_size(ewfhandle, (uint8_t*)"password", 8, &passwordsize, &ewferror);
	uint8_t* passvalue = new uint8_t[passwordsize];
	retopen = libewf_handle_get_utf8_header_value(ewfhandle, (uint8_t*)"password", 8, passvalue, passwordsize, &ewferror);
	std::string password = (char*)passvalue;
	// MODEL
	size_t modelsize = 0;
	retopen = libewf_handle_get_utf8_header_value_size(ewfhandle, (uint8_t*)"model", 5, &modelsize, &ewferror);
	uint8_t* mvalue = new uint8_t[modelsize];
	retopen = libewf_handle_get_utf8_header_value(ewfhandle, (uint8_t*)"model", 5, mvalue, modelsize, &ewferror);
	std::string model = (char*)mvalue;
	// SERIAL NUMBER
	size_t serialnumbersize = 0;
	retopen = libewf_handle_get_utf8_header_value_size(ewfhandle, (uint8_t*)"serial_number", 13, &serialnumbersize, &ewferror);
	uint8_t* snum = new uint8_t[serialnumbersize];
	retopen = libewf_handle_get_utf8_header_value(ewfhandle, (uint8_t*)"serial_number", 13, snum, serialnumbersize, &ewferror);
	std::string serialnumber = (char*)snum;
	// GET MEDIA SIZE
	size64_t imgsize = 0;
        libewf_handle_get_media_size(ewfhandle, &imgsize, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
	uint8_t format = 0;
	// FORMAT
	retopen = libewf_handle_get_format(ewfhandle, &format, &ewferror);
	// SECTORS PER CHUNK
	uint32_t sectorsperchunk = 0;
	retopen = libewf_handle_get_sectors_per_chunk(ewfhandle, &sectorsperchunk, &ewferror);
	// COMPRESSION METHOD
	uint16_t compressionmethod = 0;
	retopen = libewf_handle_get_compression_method(ewfhandle, &compressionmethod, &ewferror);
	// COMPRESSION LEVEL
	int8_t clevel = 0;
	uint8_t cflags = 0;
	retopen = libewf_handle_get_compression_values(ewfhandle, &clevel, &cflags, &ewferror);
	// MEDIA TYPE
	uint8_t mediatype = 0;
	retopen = libewf_handle_get_media_type(ewfhandle, &mediatype, &ewferror);
	// BYTES PER SECTOR
	uint32_t bytespersector = 0;
	retopen = libewf_handle_get_bytes_per_sector(ewfhandle, &bytespersector, &ewferror);
	// SECTOR COUNT
	uint64_t sectorcount = 0;
	retopen = libewf_handle_get_number_of_sectors(ewfhandle, &sectorcount, &ewferror);
	// HASH
	uint8_t* hash = new uint8_t[16];
	retopen = libewf_handle_get_md5_hash(ewfhandle, hash, 32, &ewferror);
	// CLEAN UP LIBEWF VARIABLES
        libewf_handle_close(ewfhandle, &ewferror);
        libewf_handle_free(&ewfhandle, &ewferror);
        libewf_glob_free(globfiles, globfilecnt, &ewferror);
	libewf_error_free(&ewferror);
	// FORMAT HASH
	std::stringstream ss;
	ss << std::hex << std::setfill('0');
	for(int i=0; i < 16; i++)
	    ss << std::setw(2) << (uint)hash[i];

	// POPULATE PROPERTIES
	*propstr = casenumber + ">" + description + ">" + examinername + ">" + evidencenumber + ">" + notes + ">" + acquirydate + ">" + systemdate + ">" + acquiryoperatingsystem + ">" + acquirysoftwareversion + ">" + std::to_string(format) + ">" + std::to_string(sectorsperchunk) + ">" + std::to_string(compressionmethod) + ">" + std::to_string(clevel) + ">" + std::to_string(mediatype) + ">" + std::to_string(bytespersector) + ">" + std::to_string(sectorcount) + ">" + std::to_string(imgsize) + ">" + ss.str();
    }
    else if(libvhdi_check_file_signature(imgpath->c_str(), &vhderror) == 1 || imgext.compare("vhd") == 0 || imgext.compare("vhdx") == 0 || imgext.compare("VHD") == 0 || imgext.compare("VHDX") == 0) // VHD/VHDX
    {
        libvhdi_file_t* vhdfile = NULL;
        int retopen = 0;
        retopen = libvhdi_file_initialize(&vhdfile, &vhderror);
        if(retopen == -1)
            libvhdi_error_fprint(vhderror, stdout);
        retopen = libvhdi_file_open(vhdfile, imgpath->c_str(), LIBVHDI_OPEN_READ, &vhderror);
        if(retopen == -1)
            libvhdi_error_fprint(vhderror, stdout);
	// FILE TYPE
	int filetype = 0;
	retopen = libvhdi_file_get_file_type(vhdfile, &filetype, &vhderror);
	// FORMAT VERSION
	uint16_t majorversion = 0;
	uint16_t minorversion = 0;
	retopen = libvhdi_file_get_format_version(vhdfile, &majorversion, &minorversion, &vhderror);
	// DISK TYPE
	uint32_t disktype = 0;
	retopen = libvhdi_file_get_disk_type(vhdfile, &disktype, &vhderror);
	// BYTES PER SECTOR
	uint32_t bytespersector = 0;
	retopen = libvhdi_file_get_bytes_per_sector(vhdfile, &bytespersector, &vhderror);
	// IDENTIFIER
	uint8_t* guiddata = new uint8_t[16];
	retopen = libvhdi_file_get_identifier(vhdfile, guiddata, 16, &vhderror);
	// FORMAT IDENTIFIER
	std::stringstream ss;
	ss << std::hex << std::setfill('0');
	for(int i=0; i < 16; i++)
	{
	    ss << std::setw(2) << (uint)guiddata[i];
	    if(i == 3)
		ss << "-";
	    else if(i == 5)
		ss << "-";
	    else if(i == 7)
		ss << "-";
	    else if(i == 9)
		ss << "-";
	}
	// MEDIA SIZE
	size64_t imgsize = 0;
        libvhdi_file_get_media_size(vhdfile, &imgsize, &vhderror);
        if(retopen == -1)
            libvhdi_error_fprint(vhderror, stdout);
	// VHDI CLEANUP
        libvhdi_file_close(vhdfile, &vhderror);
        libvhdi_file_free(&vhdfile, &vhderror);
        libvhdi_error_free(&vhderror);

	// PROPERTIES STRING
	*propstr = std::to_string(filetype) + ">" + std::to_string(majorversion) + "." + std::to_string(minorversion) + ">" + std::to_string(disktype) + ">" + std::to_string(imgsize) + ">" + std::to_string(bytespersector) + ">" + ss.str();
    }
    else if(libqcow_check_file_signature(imgpath->c_str(), &qcowerror) == 1 || imgext.compare("qcow") == 0 || imgext.compare("qcow2") == 0 || imgext.compare("QCOW") == 0 || imgext.compare("QCOW2") == 0) // QCOW/QCOW2
    {
	libqcow_error_t* qcowerror = NULL;
	libqcow_file_t* qcowfile = NULL;
	int ret = 0;
	ret = libqcow_file_initialize(&qcowfile, &qcowerror);
	if(ret == -1)
	    libqcow_error_fprint(qcowerror, stdout);
	ret = libqcow_file_open(qcowfile, imgpath->c_str(), LIBQCOW_OPEN_READ, &qcowerror);
	// FORMAT VERSION
	uint32_t formatversion = 0;
	ret = libqcow_file_get_format_version(qcowfile, &formatversion, &qcowerror);
	// MEDIA SIZE
	size64_t imgsize = 0;
	if(ret == -1)
	    libqcow_error_fprint(qcowerror, stdout);
	ret = libqcow_file_get_media_size(qcowfile, &imgsize, &qcowerror);
	if(ret == -1)
	    libqcow_error_fprint(qcowerror, stdout);
	// ENCRYPTION METHOD
	uint32_t encryptionmethod = 0;
	ret = libqcow_file_get_encryption_method(qcowfile, &encryptionmethod, &qcowerror);
	// SNAPSHOT COUNT
	int snapshotcount = 0;
	ret = libqcow_file_get_number_of_snapshots(qcowfile, &snapshotcount, &qcowerror);
	libqcow_file_close(qcowfile, &qcowerror);
	libqcow_file_free(&qcowfile, &qcowerror);
	libqcow_error_free(&qcowerror);

	*propstr = std::to_string(formatversion) + ">" + std::to_string(imgsize) + ">" + std::to_string(encryptionmethod) + ">" + std::to_string(snapshotcount);
    }
    else if(libvmdk_check_file_signature(imgpath->c_str(), &vmdkerror) == 1 || imgext.compare("vmdk") == 0 || imgext.compare("VMDK") == 0) // VMDK
    {
	libvmdk_handle_t* vmdkfile = NULL;
	int ret = 0;
	ret = libvmdk_handle_initialize(&vmdkfile, &vmdkerror);
	if(ret == -1)
	    libvmdk_error_fprint(vmdkerror, stdout);
	ret = libvmdk_handle_open(vmdkfile, imgpath->c_str(), LIBVMDK_OPEN_READ, &vmdkerror);
	if(ret == -1)
	    libvmdk_error_fprint(vmdkerror, stdout);
	// DISK TYPE
	int disktype = 0;
	ret = libvmdk_handle_get_disk_type(vmdkfile, &disktype, &vmdkerror);
	// MEDIA SIZE
	size64_t imgsize = 0;
	ret = libvmdk_handle_get_media_size(vmdkfile, &imgsize, &vmdkerror);
	if(ret == -1)
	    libvmdk_error_fprint(vmdkerror, stdout);
	// CONTENT IDENTIFIER
	uint32_t contentid = 0;
	ret = libvmdk_handle_get_content_identifier(vmdkfile, &contentid, &vmdkerror);
	std::stringstream cid;
	cid << "0x" << std::hex << std::setfill('0') << contentid;
	//std::cout << "content id: " << cid.str() << std::endl;
	/*
	// PARENT CONTENT IDENDITIFER
	uint32_t parconid = 0;
	ret = libvmdk_handle_get_parent_content_identifier(vmdkfile, &parconid, &vmdkerror);
	*/
	// NUMBER OF EXTENTS
	int extentcount = 0;
	ret = libvmdk_handle_get_number_of_extents(vmdkfile, &extentcount, &vmdkerror);
	libvmdk_handle_close(vmdkfile, &vmdkerror);
	libvmdk_handle_free(&vmdkfile, &vmdkerror);
	libvmdk_error_free(&vmdkerror);

	*propstr = std::to_string(disktype) + ">" + std::to_string(imgsize) + ">" + cid.str() + ">" + std::to_string(extentcount);
    }
    else if(imgext.compare("aff4") == 0 || imgext.compare("AFF4") == 0) // AFF4
    {
        AFF4_init();
        int aff4handle = AFF4_open(imgpath->c_str());
	size64_t imgsize = 0;
        imgsize = AFF4_object_size(aff4handle);
        AFF4_close(aff4handle);
	//std::cout << "image size: " << imgsize << std::endl;
	int64_t blocksize = AFF4_object_blocksize(aff4handle);
	//std::cout << "block size: " << blocksize << std::endl;
	std::string resourceid = aff4::container::getResourceID(*imgpath);
	std::cout << "resource id: " << resourceid << std::endl;
	aff4::IAFF4Resolver* aff4resolver = aff4::container::createResolver(*imgpath);
	std::shared_ptr<aff4::IAFF4Container> aff4container = aff4::container::openAFF4Container(*imgpath, aff4resolver);
	std::vector<std::shared_ptr<aff4::IAFF4Image>> imglist = aff4container->getImages();
	for(int i=0; i < imglist.size(); i++)
	{
	    std::map<aff4::Lexicon, std::vector<aff4::rdf::RDFValue>> aff4properties = imglist.at(i)->getProperties();
	    std::cout << "aff4 property count: " << aff4properties.size() << std::endl;
	    for(int j=0; j < aff4properties.size(); j++)
	    {
	    }
	    //std::shared_ptr<aff4::IAFF4Map> aff4map = imglist.at(i)->getMap();
	    //std::cout << "img size: " << aff4map->size() << std::endl;
	    //std::shared_ptr<aff4::IAFF4Stream> aff4stream = aff4map->getStream();
	    //std::cout << "img size: " << aff4stream->size() << std::endl;
	}
	//std::cout << imgfile << " size: " << imgsize << std::endl;
    }
    /*
    libqcow_error_t* qcowerr = NULL;
    libvmdk_error_t* vmdkerr = NULL;
    libphdi_error_t* phderr = NULL;
    // SET IMGTYPE - DETERMINES HOW READ THE RAW CONTENT
    if(imgext.compare("dd") == 0 || imgext.compare("DD") == 0) // RAW
        imgtype = 1;
    else if(imgext.compare("000") == 0 || imgext.compare("001") == 0) // SPLIT RAW
        imgtype = 4;
    else if(imgext.compare("wfi") == 0) // WFI
        imgtype = 5;
    else if(imgext.compare("wli") == 0) // WLI
        imgtype = 6;
	imgtype = 8;
	imgtype = 9;
    else if(libphdi_check_file_signature(imgfile.c_str(), &phderr) == 1 || imgext.compare("phd") == 0 || imgext.compare("PHD") == 0) // PHD
	imgtype = 10;
    else // ANY OLD FILE
        imgtype = 0;
    libqcow_error_free(&qcowerr);
    libvmdk_error_free(&vmdkerr);
    libphdi_error_free(&phderr);
    // SET IMGSIZE - GET THE SIZE OF THE RAW CONTENT
    if(imgtype == 1) // RAW
    {
        imagebuffer.open(imgfile.c_str(), std::ios::in|std::ios::binary);
        imagebuffer.seekg(0, imagebuffer.beg);
        imagebuffer.seekg(0, imagebuffer.end);
        imgsize = imagebuffer.tellg();
        imagebuffer.close();
        //std::cout << imgfile << " size: " << imgsize << std::endl;
    }
    else if(imgtype == 3) // AFF4
    {
        AFF4_init();
        int aff4handle = AFF4_open(imgpath.c_str());
        imgsize = AFF4_object_size(aff4handle);
        AFF4_close(aff4handle);
	//std::cout << imgfile << " size: " << imgsize << std::endl;
    }
    else if(imgtype == 4) // SPLIT RAW
    {
	libsmraw_handle_t* smhandle = NULL;
	libsmraw_error_t* smerror = NULL;
	char** globfiles = NULL;
	int globfilecnt = 0;
	int found = imgpath.rfind(".0");
	if(found == -1)
	    found = imgpath.rfind(".a");
	std::string imgprematch = imgpath.substr(0, found+2);
	std::filesystem::path imagepath(imgpath);
	imagepath.remove_filename();
	for(const auto &file : std::filesystem::directory_iterator(imagepath))
	{
	    if(file.path().string().compare(0, found+2, imgprematch) == 0)
		globfilecnt++;
	}
	char* filenames[globfilecnt] = {NULL};
	filenames[0] = (char*)imgpath.c_str();
	int i = 1;
	for(const auto &file : std::filesystem::directory_iterator(imagepath))
	{
	    if(file.path().string().compare(0, found+2, imgprematch) == 0)
	    {
		if(file.path().string().compare(imgpath) != 0)
		{
		    filenames[i] = (char*)file.path().string().c_str();
		    i++;
		}
	    }
	}
	int retopen = 0;
	retopen = libsmraw_glob(filenames[0], strlen(filenames[0]), &globfiles, &globfilecnt, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
	retopen = libsmraw_handle_initialize(&smhandle, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
	retopen = libsmraw_handle_open(smhandle, globfiles, globfilecnt, LIBSMRAW_OPEN_READ, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
	libsmraw_handle_get_media_size(smhandle, &imgsize, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
	libsmraw_handle_close(smhandle, &smerror);
	libsmraw_handle_free(&smhandle, &smerror);
	libsmraw_glob_free(globfiles, globfilecnt, &smerror);
	libsmraw_error_free(&smerror);
        //std::cout << imgfile << " size: " << imgsize << std::endl;
    }
    else if(imgtype == 5) // WFI
    {
	FILE* wfifile = NULL;
	wfifile = fopen(imgpath.c_str(), "rb");
	fseek(wfifile, 0, SEEK_END);
	fseek(wfifile, -264, SEEK_CUR);
	fread(&wfimd, sizeof(struct wfi_metadata), 1, wfifile);
	fclose(wfifile);
        imgsize = wfimd.totalbytes;
	//std::cout << imgpath << " size: " << wfimd.totalbytes << std::endl;
	/*
	printf("\nwombatinfo v0.1\n\n");
	printf("Raw Media Size: %llu bytes\n", wfimd.totalbytes);
	printf("Case Number:\t %s\n", wfimd.casenumber);
	printf("Examiner:\t %s\n", wfimd.examiner);
	printf("Evidence Number: %s\n", wfimd.evidencenumber);
	printf("Description:\t %s\n", wfimd.description);
	printf("BLAKE3 Hash:\t ");
	for(size_t i=0; i < 32; i++)
	    printf("%02x", wfimd.devhash[i]);
	printf("\n");
	 */ 
/*    }
    else if(imgtype == 6) // WLI
    {
    }
    else if(imgtype == 8) // QCOW/QCOW2
    {
	libqcow_error_t* qcowerror = NULL;
	libqcow_file_t* qcowfile = NULL;
	int ret = 0;
	ret = libqcow_file_initialize(&qcowfile, &qcowerror);
	if(ret == -1)
	    libqcow_error_fprint(qcowerror, stdout);
	ret = libqcow_file_open(qcowfile, imgpath.c_str(), LIBQCOW_OPEN_READ, &qcowerror);
	if(ret == -1)
	    libqcow_error_fprint(qcowerror, stdout);
	ret = libqcow_file_get_media_size(qcowfile, &imgsize, &qcowerror);
	if(ret == -1)
	    libqcow_error_fprint(qcowerror, stdout);
	libqcow_file_close(qcowfile, &qcowerror);
	libqcow_file_free(&qcowfile, &qcowerror);
	libqcow_error_free(&qcowerror);
    }
    else if(imgtype == 9) // VMDK
    {
	libvmdk_error_t* vmdkerror = NULL;
	libvmdk_handle_t* vmdkfile = NULL;
	int ret = 0;
	ret = libvmdk_handle_initialize(&vmdkfile, &vmdkerror);
	if(ret == -1)
	    libvmdk_error_fprint(vmdkerror, stdout);
	ret = libvmdk_handle_open(vmdkfile, imgpath.c_str(), LIBVMDK_OPEN_READ, &vmdkerror);
	if(ret == -1)
	    libvmdk_error_fprint(vmdkerror, stdout);
	ret = libvmdk_handle_get_media_size(vmdkfile, &imgsize, &vmdkerror);
	if(ret == -1)
	    libvmdk_error_fprint(vmdkerror, stdout);
	libvmdk_handle_close(vmdkfile, &vmdkerror);
	libvmdk_handle_free(&vmdkfile, &vmdkerror);
	libvmdk_error_free(&vmdkerror);
    }
    else if(imgtype == 10) // PHD
    {
	libphdi_error_t* phdierror = NULL;
	libphdi_handle_t* phdifile = NULL;
	int ret = 0;
	ret = libphdi_handle_initialize(&phdierror, &phdierror);
	if(ret == -1)
	    libphdi_error_fprint(phdierror, stdout);
	ret = libphdi_handle_open(phdifile, imgpath.c_str(), LIBPHDI_OPEN_READ, &phdierror);
	if(ret == -1)
	    libphdi_error_fprint(phdierror, stdout);
	ret = libphdi_handle_get_media_size(phdifile, &imgsize, &phdierror);
	if(ret == -1)
	    libphdi_error_fprint(phdierror, stdout);
	libphdi_handle_close(phdifile, &phdierror);
	libphdi_handle_free(&phdifile, &phdierror);
	libphdi_error_free(&phdierror);
    }
     */ 
}

