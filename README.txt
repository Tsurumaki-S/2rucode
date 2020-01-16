
#----------------#
   コンパイル方法
#----------------#

	通常版
	$ bash make.sh

	MPI並列版
	$ bash make_mpi.sh

## 注意 ##
	上のコマンドを入力すると、"makefile"が作成されるが、
	ソースコードを各自で改変しコンパイルする際には、
		$ make
	としてはいけない。
		$ bash make.sh
	または、
		$ bash make_mpi.sh
	とすること。


#---------#
   実行例
#---------#

	通常版
	$ 2ru (引数)

	MPI並列版
	$ mpirun -n 4 2ru_mpi (引数)

