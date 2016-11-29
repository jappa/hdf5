#ifndef hdffileH
#define hdffileH

#include "hdfgroup.hpp"
#include "hdf5/traits.hpp"
#include <string>

namespace hdf {
template<class HDFImpl=HDF5Traits>
class HDFFile : public HDFGroup<HDFImpl> {
  public:
    typedef HDFGroup<HDFImpl> HDFGroup_t;
    enum Flags {
      none = 0x0,
      truncate = 0x1,
      readonly = 0x2
    };


    /**
     * Open the hdf5 file at the given location
     * Truncates the file if flags == truncate
     */
    HDFFile(const std::string & path, Flags flags = none) {
      file = HDFImpl::open(path, flags&truncate, flags&readonly);
      HDFGroup_t::initFileGroup(*file);
    };

    ~HDFFile() {
        HDFGroup<HDFImpl>::group.reset();
    }

    void flush() {
        file->flush();
    }

  private:
    std::shared_ptr<typename HDFImpl::file_handle_type> file;
};

#ifdef H5_HAVE_PARALLEL
template<class HDFImpl=HDF5Traits>
class HDFParallelFile : public HDFGroup<HDFImpl> {
  public:
    typedef HDFGroup<HDFImpl> HDFGroup_t;
    enum Flags {
      none = 0x0,
      truncate = 0x1,
      readonly = 0x2
    };


    /**
     * Open the hdf5 file at the given location
     * Truncates the file if flags == truncate
     */
    HDFParallelFile(const std::string & path, Flags flags = none, MPI_Comm mpi_comm=MPI_COMM_WORLD) {
      file = HDFImpl::parallel_open(path, flags&truncate, flags&readonly, mpi_comm);
      HDFGroup_t::initFileGroup(*file);
    };

    ~HDFParallelFile() {
        HDFGroup<HDFImpl>::group.reset();
    }

    void flush() {
        file->flush();
    }

  private:
    std::shared_ptr<typename HDFImpl::file_handle_type> file;
};
#endif
}

#endif

