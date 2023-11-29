#include<iostream>
#include<cmath>

class Matrix{
private:
    double* arr;
    int column, row;
    int issquare;
public:
    Matrix(double* arr, int rows, int column) {
        this->arr = arr;
        this->column = column;
        this->row = rows;
        issquare = (row == column);
    }
    Matrix(){}
    void display(){
        for(int i=0;i<column;i++){
            for(int j=0;j<row;j++){
                printf("%f ",*(arr+i*row+j));
            }
            printf("\n");
        }
    }
    void displayS(double* arr,int row,int column){
        for(int i=0;i<column;i++){
            for(int j=0;j<row;j++){
                printf("%.3f ",*(arr+i*row+j));
            }
            printf("\n");
        }
    }
    double* returnMatrix(){
        return arr;
    }
    int returnRow(){
        return row;
    }
    int returnColumn(){
        return column;
    }
    void changeMatrix(double* ptr){
        arr = ptr;
    }
    Matrix orthonormal(double* ptr1,int row,int column){
        double* ptr = (double*)malloc(sizeof(double)*row*column);
        for(int i=0;i<row*column;i++){
            *(ptr+i) = *(ptr1+i);
        }
        Matrix m = Matrix(ptr,row,column);
        double* cptr = (double*)malloc(sizeof(double)*row*column);
        for(int i=0;i<row*column;i++){
            *(cptr+i) = *(ptr+i);
        }
        Matrix cm = Matrix(cptr,row,column);
        double sum=0;
        for(int j= 0;j<row;j++){
            sum += pow(*(cptr+j*row),2); 
        }
        sum = sqrt(sum);
        for(int j = 0;j<row;j++){
            *(ptr+j*row) /= sum;
        }
        double dot=0,jLength = 0;
        for(int i = 1;i<column;i++){
            for(int j=0;j<i;j++){
                dot = 0;
                jLength = 0;
                for(int k = 0;k<row;k++){
                    dot += *(ptr+k*row+j)**(cptr+k*row+i);
                    jLength += *(ptr+k*row+j)**(ptr+k*row+j);
                }
                dot = dot/sqrt(jLength);
                for(int k =0;k<column;k++){
                    *(ptr+k*row+i) -= dot**(ptr+k*row+j);
                }
            }
            sum=0;
            for(int j= 0;j<row;j++){
                sum += pow(*(ptr+j*row+i),2); 
            }
            sum = sqrt(sum);
            for(int j = 0;j<row;j++){
                *(ptr+j*row+i) = *(ptr+j*row+i)/sum;
            }
            
        }
        return m;

    }
    Matrix operator+(Matrix m){
        if(row!=m.returnRow() || column!=m.returnColumn()){
            throw "Dimentions Doesnt Match";
        }
        double* n = (double*)malloc(sizeof(double)*row*column);
        for(int i = 0;i<row*column;i++){
            *(n+i) = *(arr+i)+*(m.returnMatrix()+i);
        }
        return Matrix(n,row,column);
    }
    Matrix operator-(Matrix m){
        if(row!=m.returnRow() || column!=m.returnColumn()){
            throw "Dimentions Doesnt Match";
        }
        double* n = (double*)malloc(sizeof(double)*row*column);
        for(int i = 0;i<row*column;i++){
            *(n+i) = *(arr+i)-*(m.returnMatrix()+i);
        }
        return Matrix(n,row,column);
    }
    Matrix transpose(){
        double* ptr = (double*)calloc(row*column,sizeof(double));
        for(int i=0;i<column;i++){
            for(int j=0;j<row;j++){
                *(ptr+j*row+i) = *(arr+i*row+j);
            }
        }
        return Matrix(ptr,column,row);
    }
    Matrix operator*(double k){
        double* f = (double*)calloc(row*column,sizeof(double));
        for (int i=0;i<row*column;i++){
            *(f+i) = k**(arr+i);
        }
        return Matrix(f,row,column);
    }
    double determinant(double* ptr,int size){
        if(issquare==0){
            throw "It Must Be A Square";
        }
        if(size == 1){
            return *ptr;
        }
        if(size == 4){
            return *(ptr)**(ptr+3)-*(ptr+1)**(ptr+2);
        }
        double sum = 0;
        double* pptr;
        int pos;
        pptr = (double*)calloc(pow(sqrt(size)-1,2),sizeof(double));
        for(int i = 0;i<sqrt(size);i++){
            pos = 0;
            for (int j = 1;j<sqrt(size);j++){
                for(int k=0;k<sqrt(size);k++){
                    if(k!=i){
                        *(pptr+pos) = *(ptr+j*(int)sqrt(size)+k);
                        pos++;
                    }
                }
            }
            sum += pow(-1,i)**(ptr+i)*determinant(pptr,pow(sqrt(size)-1,2));
        }
        return sum;
    }
    int check_lower_triangular_matrix(double* mat,int N)
    {
	    int i, j;
	    for (i = 1; i < N; i++){
		    for (j = 0; j < i; j++){
			    if (abs(*(mat+i*N+j)) != 0){
				    return 0;
                }
            }
        }
        return 1;
    }
    /*
    Matrix ModelMatrix(){
        double* eigenvalue = (double*)malloc(sizeof(double)*row);
        EigenValues(eigenvalue,20);
        Matrix M = Matrix((double*)malloc(sizeof(double)*row*column),row,column);
        Matrix A = Matrix((double*)malloc(sizeof(double)*(row-1)*(column-1)),row-1,column-1);
        Matrix B;
        Matrix C = Matrix((double*)malloc(sizeof(double)*row),row,1);
        
        double* ptr = (double*)malloc(sizeof(double)*row);
        for(int i = 0;i<row;i++){
            for(int j= 0;j<row-1;j++){
                for(int k=0;j<row-1;k++){
                    *(A.returnMatrix()+j*(row-1)+k) = *(arr+j*row+k);
                }
            }
            A = *this-Matrix::Identity(row-1)**(eigenvalue+i);
            B = Matrix(ptr,row-1,1);
            for(int j = 0;j<row-1;j++){
                *(B.returnMatrix()+j) = *(arr+j*row+row);
            }
            C = A.inverse()*B;
            for(int j = 0;j<row-1;j++){
                *(M.returnMatrix()+j*row+i) = *(C.returnMatrix()+j);
            }
        }
        return M;
    }*/
    double* EigenValues(int iter){
        Matrix A = *this;
        Matrix Q = A.orthonormal(A.returnMatrix(),A.returnRow(),A.returnColumn());
        Matrix R = Q.transpose()*A;
        int h = 0;
        while(1){
            h++;
            printf("1");
            if(A.check_lower_triangular_matrix(A.returnMatrix(),A.returnColumn()) && h>iter){
                break;
            }
            A = R*Q;

            Q = A.orthonormal(A.returnMatrix(),A.returnRow(),A.returnColumn());
            R = Q.transpose()*A;
            A = Q*R;
        }
        
        int pos = 0;
        double* ptr = (double*)malloc(A.returnRow()*sizeof(double));
        for(int i = 0 ;i<A.returnRow();i++){
            for(int j = 0;j<A.returnColumn();j++){
                if(i==j){
                    *(ptr+pos) = *(A.returnMatrix()+i*A.returnRow()+j);
                    pos++;
                }
            }
        }
        return ptr;
    }
    Matrix adjoint(){
        if(issquare==0){
            return Matrix((double*)malloc(4),1,1);
        }
        int pos ;
        double* ptr = (double*)calloc(row*column,sizeof(double));
        double* pptr = (double*)calloc((row-1)*(column-1),sizeof(double));
        for(int i=0;i<row;i++){
            for(int j=0;j<column;j++){
                pos = 0;
                for(int k=0;k<row;k++){
                    for(int l=0;l<column;l++){
                        if(i!=k && j!=l){
                            *(pptr+pos) = *(arr+k*row+l);
                            pos++; 
                        }
                    }
                }
                *(ptr+i*row+j) = pow(-1,i+j)*determinant(pptr,(row-1)*(row-1));
            }
        }
        Matrix m = Matrix(ptr,row,column);
        m = m.transpose();
        return m;

    }
    Matrix inverse(){
        if(issquare==0){
            return Matrix((double*)calloc(1,sizeof(double)),1,1);
        }
        double detinv = (1/(determinant(arr,row*column)));
        return adjoint()*detinv;

    }
    static Matrix Identity(int n){
        double* ptr = (double*)calloc(n*n,sizeof(double));
        for(int i= 0;i<n;i++){
            for(int j =0;j<n;j++){
                if(i==j){
                    *(ptr+n*i+j) = 1;
                }
            }
        }
        return Matrix(ptr,n,n);
    }
    Matrix operator*(Matrix m){
        Matrix ret = Matrix((double*)calloc(this->row*m.returnColumn(),sizeof(double)),this->row,m.returnColumn());
        double* ptr = (double*)calloc(this->row*m.returnColumn(),sizeof(double));
        if(this->column!=m.returnRow()){
            return ret;
        }
        for(int i=0;i<this->column;i++){
            for(int j=0;j<m.returnRow();j++){
                for(int k = 0;k<this->column;k++){
                    *(ptr+i*this->returnRow()+j) += *(this->arr+k+i*this->column) * *(m.returnMatrix()+k*m.returnRow()+j);
                }
            }
        }
        ret.changeMatrix(ptr);
        return ret;
    }

};

int main(){
    double a[9] = {5,-10,5,2,14,2,-4,-8,6};
    Matrix m = Matrix(a,3,3);
    double *p = m.EigenValues(2);
    for(int i =0 ;i<3;i++){
        printf("%lf ",p[i]);
    }
}