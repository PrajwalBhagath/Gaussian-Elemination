#include <iostream>
#include <vector> 
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <ctime>



using namespace std;

class MyMatrix
{
private:
	vector<double> elements; // array containing data
	vector<double*> rows; // array of pointers to start of each row
	unsigned ncols; // number of columns
	unsigned nrows;
public:
	// constructor takes matrix dimensions as arguments
	MyMatrix(unsigned nrows1, unsigned ncols1);
	// method returns the element in the ith row and jth column
	double get(unsigned irow, unsigned jcol);
	// method sets the element in the ith row and jth column
	void set(unsigned irow, unsigned jcol, double value);
	//swap the ith row and jth column
	void interchange(unsigned irow, unsigned jrow);
	//multiply the ith row by x
	void multiply(unsigned irow, double x);
	void divide(unsigned irow, double x);
	//add x times the jth row to the ith row
	void add(unsigned irow, double x, unsigned jrow);
	void fill();
	void print();
	void invert(MyMatrix A);
	void matrixMultipy(MyMatrix mat1, MyMatrix mat2);
};

int main()
{
	// construct a new instance/object of the class MyMatrix
	unsigned nrows = 13;
	unsigned ncols = 13;
	srand(time(0));
	MyMatrix matrix(nrows, ncols);//setting up a new matrix with random value
	matrix.fill();
	cout << "the original matrix" << endl;
	matrix.print();
	matrix.invert(matrix);


};

void MyMatrix::invert(MyMatrix A) {

	MyMatrix original(13, 13);//setting a temporary matrix with the samw values
	MyMatrix inverse(13, 13);//settng the a new matrix to be modifies
	MyMatrix identity(13, 13);//making a new identity matrix

	//filling in values of the identity matrix
	for (int i = 0; i < A.nrows; i++) {
		for (int j = 0; j < A.nrows; j++) {
			
			identity.set(i, j, 0.0);

			if (i == j) {
				identity.set(i, j, 1);
			}
		}
	}

	cout << "ID Matrix" << endl;// printing the id matrix
	identity.print();

	//setting the values for the temporary matrix
	for (int i = 0; i < A.nrows; i++) {
		for (int j = 0; j < A.ncols; j++) {
			original.set(i, j, (A.get(i, j)));
		}
	}

	//setting the values for the matrix that is going to be used to perform the gaussian elemination
	for (int i = 0; i < A.nrows; i++) {
		for (int j = 0; j < A.ncols; j++) {
			inverse.set(i, j, (A.get(i, j)));
		}
	}

	//pivoting to get the higest value in the pivot
	for (int i = 0; i < A.nrows; i++)
	{
		for (int j = i + 1; j < A.nrows; j++)//checks the value of the below the matrix leader
		{
			if (inverse.get(i, i) < inverse.get(j, i))
			{
				inverse.interchange(i, j);//interchanging the rows for the inverse matrix to get the higest value for the pivot
				identity.interchange(i, j);//doing the same for the established identity matrix
			}
		}
	}

	//printinf the matrix after pivotisation
	std::cout << "After pivot" << endl;
	inverse.print();

	unsigned pivot_row = 0;//the element id for the pivot row
	unsigned pivot_col = 0;//the element id for the pivot column

	//begening a while loop to make sure the process loops over and over until the end of the matrix
	while (pivot_col < A.ncols && pivot_row < A.nrows)//so it does not surpass the number of rows or columns
	{
		double pivot_val = inverse.get(pivot_row, pivot_col);//sets a temporary value for the pivot value as it goes through each column
		
		//for loop to go to each element in the column row by row 
		//unless it is the pivot value
		for (int j = 0; j < A.nrows; j++) {
			if (j == pivot_row) 
			{
				continue;//skips the pivot
			}
			else 
			{
				double scale = -inverse.get(j, pivot_col) / pivot_val;//a/b
				inverse.add(j, scale, pivot_row);//adding the value calculated to the whole row
				identity.add(j, scale, pivot_row);//doing the same to our identity matrix

				//inverse.set(j, pivot_col, 0.0);
			}
		}

		pivot_col++;//increase the column number for the time the next time the while loop begins
		pivot_row++;//increases the row number for the next time the while loop begins
	}
	
	for (int j = 0; j < A.nrows; j++) {
		identity.divide(j, inverse.get(j, j));//divides the whole value of the identity matrix by the pivot value 
		inverse.divide(j, inverse.get(j, j));//divides the whole row by the value of the prvot to make it equal one
	}

	//printing the result of the gaussian elemination
	std::cout << "After elemination" << endl;
	inverse.print();

	//printing the result of the identity matrix after performing the same methods as the inverse martrix
	//that has gone through the gaussian eleimination process
	cout << "ID Matrix after elimination" << endl;
	identity.print();

	//multiplying the identity(inverse matrix now) to the original matrix 
	matrixMultipy(original, identity);
}


MyMatrix::MyMatrix(unsigned ncols1, unsigned nrows1)
{
	ncols = ncols1;
	nrows = nrows1;

	elements.resize(nrows * ncols);//resizing elements to nrows*ncols
	rows.resize(nrows);//resize rows to the number of rows
	for (unsigned i = 0; i < nrows; i++)
	{
		rows[i] = &(elements[i * ncols]);
	}
}

double MyMatrix::get(unsigned irow, unsigned jcol)
{
	return this->rows[irow][jcol];
}

void MyMatrix::set(unsigned irow, unsigned jcol, double value)
{
	rows[irow][jcol] = value;//setting the value at the ith row and the jth column
}

void MyMatrix::interchange(unsigned irow, unsigned jrow)
{
	vector<double*> rows_temp;//making temporary rows 
	rows_temp = rows;//setting to to the vector rows
	rows[irow] = rows_temp[jrow];//replacing the ith row with the temporary jth row
	rows[jrow] = rows_temp[irow];//replacing the jth row with the temporary ith row
}

void MyMatrix::multiply(unsigned irow, double x)
{
	for (unsigned j = 0; j < ncols; j++)//loop to go column by column on the same row
	{
		rows[irow][j] = rows[irow][j] * x;//myltiplying the constant to each number on the same row but diffrent columns
	}
}

void MyMatrix::divide(unsigned irow, double x)
{
	for (unsigned j = 0; j < ncols; j++)//loop to go column by column on the same row
	{
		rows[irow][j] = rows[irow][j] / x;//myltiplying the constant to each number on the same row but diffrent columns
	}
}

void MyMatrix::add(unsigned irow, double x, unsigned jrow) //adding matrix
{

	for (unsigned i = 0; i < ncols; i++)//loops for going column by column
	{
		double value = get(irow, i) + (x * get(jrow, i));//adding each number in the row one by one
		set(irow, i, value);//replacing the value of the matrix to the new value
	}

}

void::MyMatrix::fill()
{
	for (int i = 0; i < 13; i++)

	{
		for (int j = 0; j < 13; j++) {

			double random = (double)rand() / RAND_MAX;
			random = random * (100);
			rows[i][j] = round(random);
		}

	}

}


void::MyMatrix::print()
{

	for (int i = 0; i < nrows; i++)
	{
		for (int j = 0; j < ncols; j++)
		{
			std::cout << setw(10) << setprecision(2) << fixed << get(i, j);
		}
		std::cout << endl;
	}
}

void::MyMatrix::matrixMultipy(MyMatrix mat1, MyMatrix mat2) {
	MyMatrix id(13, 13);

	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			double value = 0;
			for (int k = 0; k < 13; k++)
			{
				value += mat1.get(i, k) * mat2.get(k, j); //Sums the multiplication of a row value of original matrix-
				id.set(i, j, value);                     //with the equivilant column value of the inverse matrix
			}
		}
	}

	cout << "Afer multiplying with original" << endl;
	id.print();
}