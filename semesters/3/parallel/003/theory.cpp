
int MPI_Comm_size(MPI_Comm comm, int *size);  // кол-во процессов в группе
int MPI_Comm_rank(MPI_Comm comm, int *rank);  // номера процессов в группе

// Каждое сообщение в MPI имеет tag > 0

// _____________
// |  |  |  |  |
// ^
// |
// addr   nitems   datatype

int MPI_Send(void *buf, int nitems,
             MPI_Datatype data_type /* MPI_Int или MPI_Float*/, int rank_to,
             int tag, MPI_Comm comm);

int MPI_Recv(void *buf, int maxnitems,
             MPI_Datatype data_type /* MPI_Int или MPI_Float*/,
             int rank_from /* MPI_ANY_SOURCE */, int what_tag /* MPI_ANY_TAG */,
             MPI_Comm comm, MPI_Status *state);

int MPI_Status st;
st.MPI_SOURCE;
st.MPI_TAG;

// MPI_Get_count;