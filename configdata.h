#ifndef __CONFIGDATA_H__
#define __CONFIGDATA_H__

class CONFIGDATA
{
public:
    CONFIGDATA();
    ~CONFIGDATA();

    int Load();
    int Save();

private:
    void SetDefaults();
    char configPath[1024];
    
    // Helper to get the path
    void InitPath();
};

#endif
