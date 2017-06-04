#ifndef _TAG_H_
#define _TAG_H_
#ifdef __cplusplus
extern "C" {
#endif
const char* getTitleTag(const char *filename);
const char* getArtistTag(char *filename);
const char* getAlbumTag(char *filename);
const char* getGenreTag(char *filename);

#ifdef __cplusplus
}
#endif
#endif //_TAG_H_