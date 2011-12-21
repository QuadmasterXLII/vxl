// This is brl/bbas/bhdfs/bhdfs_manager.h
#ifndef bhdfs_manager_h_
#define bhdfs_manager_h_
//:
// \file
// \brief A parent class for singleton hadoop file system managers
// \author Ozge C. Ozcanli
// \date November 30, 2011
//
// \verbatim
//  Modifications
//   <none yet>
// \endverbatim


//makes a bocl_mem a sptr
#include <vbl/vbl_ref_count.h>
#include <vbl/vbl_smart_ptr.h>
#include <vsl/vsl_binary_io.h>
#include <vcl_vector.h>

#include "hdfs.h"

class bhdfs_manager;
typedef vbl_smart_ptr<bhdfs_manager> bhdfs_manager_sptr;

class bhdfs_manager_destroyer;

class bhdfs_manager: public vbl_ref_count
{
 public:

  //: Use this to access singleton instance
  static bhdfs_manager_sptr instance();

  //: create function used instead of constructor
  //  pass 'default' as host (and port as 0) to use the 'configured' filesystem in ${HADOOP_HOME}/conf/core-default.xml
  //  pass NULL as host (and port as 0) to use the filesystem on localhost 
  static void create(vcl_string host_name, int port);
  static void create(vcl_string host_name, int port, vcl_string user_name);

  static bool exists() { return bhdfs_manager::instance_!=0; }
  
  //: the destructor instance to make sure memory is deallocated when the program exits
  static bhdfs_manager_destroyer destroyer_;  // its not a pointer so C++ will make sure that it's destructor will be called
  friend class bhdfs_manager_destroyer;

  //: check if FS is opened fine
  bool ok() { return fs_ != 0; }

  //: get the current working directory on hdfs
  vcl_string get_working_dir();

  //: delete the file, returns true on success, false on error. 
  bool rm(vcl_string path);
  
  //: create a directory, returns true on success, false on error. create permission is rwx rwx --- by default 
  bool create_dir(vcl_string path, short mode =  S_IRWXU | S_IRWXG);
  
  //: remove a directory, returns true on success, false on error.  
  bool remove_dir(vcl_string path) { return rm(path); }

  //: check existence of a file or path,  returns true on success, false on error. 
  bool exists(vcl_string path);
  
  //: copy file from local dir to hdfs folder
  bool copy_to_hdfs(vcl_string local_file, vcl_string hdfs_folder);
  
  //: copy file from hdfs to local dir, hdfs_file is the full path of the file on hdfs
  bool copy_from_hdfs(vcl_string hdfs_file, vcl_string local_dir);
  
  //: get a list of filenames in the given directory
  bool get_dir_list(vcl_string dir, vcl_vector<vcl_string>& fnames); 

  friend class bhdfs_fstream;

 protected:

  //: hidden constructor
  bhdfs_manager(vcl_string host_name, int port);
  bhdfs_manager(vcl_string host_name, int port, vcl_string user_name);

  //: hidden destructor
  virtual ~bhdfs_manager() {}

  //Singleton instance of the manager
  static bhdfs_manager_sptr instance_;

  //: the file system handle for hdfs (its type is void*)
  hdfsFS fs_; 
  
};

//: Binary write bhdfs_manager to stream
void vsl_b_write(vsl_b_ostream& os, bhdfs_manager const& scene);
void vsl_b_write(vsl_b_ostream& os, const bhdfs_manager* &p);
void vsl_b_write(vsl_b_ostream& os, bhdfs_manager_sptr& sptr);
void vsl_b_write(vsl_b_ostream& os, bhdfs_manager_sptr const& sptr);

//: Binary load boxm2_scene scene from stream.
void vsl_b_read(vsl_b_istream& is, bhdfs_manager &scene);
void vsl_b_read(vsl_b_istream& is, bhdfs_manager* p);
void vsl_b_read(vsl_b_istream& is, bhdfs_manager_sptr& sptr);
void vsl_b_read(vsl_b_istream& is, bhdfs_manager_sptr const& sptr);


//: create another class whose sole purpose is to destroy the singleton instance
class bhdfs_manager_destroyer
{
 public:
  bhdfs_manager_destroyer(bhdfs_manager_sptr s = 0);
  ~bhdfs_manager_destroyer();

  void set_singleton(bhdfs_manager_sptr s);
 private:
  bhdfs_manager_sptr s_;
};

#endif // bhdfs_manager_h_