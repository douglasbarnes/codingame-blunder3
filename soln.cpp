// We are given a bunch of (n,t) coordinates corresponding to the runtime of a particular algorithm given input size n. We are told find the best fitting
// algorithmic complexity (out of a few potentital candidates). For each complexity function f, the solution finds C such that Cf has minimal quadratic
// error when approximating the input (C can be determined arbitrarily close to the true minimum). 
#include <iostream>
#include<cmath>
using namespace std;

// Use an enum so one function can handle all of our complexities instead of having a function for each. "Last" is not a complexity, but is a trick for
// for loops later.
enum complexity { c, logn, n, nlogn, n2, n2logn, n3, expo, last };

// Given arrays (ai, bi) and (ci, di) of size n, computes sqrt( sum_ij (bi-di)^2 ) /n.
double get_quadratic_loss(double data[][2],double test[][2],int n)
{
    double loss = 0;
    for(int i=0;i<n;i++)
    {
        loss += pow(data[i][1] - test[i][1],2)/(n*n);
    }
    return sqrt(loss);
}

// Given a complexity function f and a constant C, computes the quadratic error of Cf in approximating the input data
double get_error(int num, double data[][2], double C,complexity comp)
{
    double test[num][2];
  
    // Fill test[i][1] with the values of Cf at data[i][0] (the n coordinate)
    for(int i=0;i<num;i++)
    {
        // n-coordinate
        double x =  data[i][0];
        switch(comp)
            {
                case c:
                    test[i][1] = C;
                    break;
                case logn:
                    test[i][1] = C*log(x);
                    break;
                case n:
                    test[i][1] = C*x;
                    break;
                case nlogn:
                    test[i][1] = C*x*log(x);
                    break;
                case n2:
                    test[i][1] = C*pow(x,2);
                    break;
                case n2logn:
                    test[i][1] = C*pow(x,2)*log(x);
                    break;
                case n3:
                    test[i][1] = C*pow(x,3);
                    break;
                case expo:
                    test[i][1] = C*pow(2,x);
                    break;

            }

    }
    return get_quadratic_loss(data, test, num);
}

// Return the string name of a complexity
string get_string(complexity comp)
{    
    switch(comp)
    {  
        case c:
            return "O(1)";
        case logn:
            return "O(log n)";
        case n:
            return "O(n)";
        case nlogn:
            return "O(n log n)";
        case n2:
            return "O(n^2)";
        case n2logn:
            return "O(n^2 log n)";
        case n3:
            return "O(n^3)";
        case expo:
            return "O(2^n)";
        default:
            return "X";
    }

}
double get_const(double data[][2], int num, complexity comp)
{
    // Find the best constant C for which Cf(x) minimises get_quadratic_loss on the data set
    // where f(x) corresponds to the "comp" input

    // First we want to find an initial interval [0, upperC] in which the true minimising value of C must lie. 
    // Since all our functions are increasing for n>1 we can find C such that Cf(data[i][0]) > 2*data[i][1] for all i
    // in particular C = 2 * (max t)/ f (min n) will do
    // even better, we know f(n) >= 1 for all f for all n>2 
    // so upperC = 2*max(t) will do
    double upperC = 2 * ceil(data[num-1][1]);
    double lowerC = 0;
    // Secondly we discretize the C-interval [0, upperC] into p very small steps C_1,..., C_p and find which C_i minimises the error
    double p = pow(2,5); 
    // Thirdly we repeat on the interval [C_(i-1),C_(i+1)] and this process repeats until the change in error percentage is sufficiently small
    double deltaerror = 1;
    // (Generally requires 2-3 repetitions and in practice is unlikely needed to determine the complexity)
  
    // Current best value of C
    double best = -1;
    // Its quadratic error
    double besterr = -1;

    // While error is changing a lot each iteration,
    while(deltaerror > 0.00001)
    {        
        double olderr = besterr;
        for(int i=1;i<p-1; i++)
        {
            // newC = C_i
            double newC = lowerC + (i/p)*(upperC-lowerC);
            // Its error
            double newerr = get_error(num,data,newC,comp);            
            // Check if better than best
            if(newerr < besterr || besterr == -1)
            {
                best = newC;
                besterr = newerr;                
            }
        }
        // Update change in error
        deltaerror =  1- besterr/olderr;

        // Set new search interval to be the interval surrounding best
        lowerC = best - (1/p)*(upperC-lowerC);
        upperC = best + (1/p)*(upperC-lowerC);
    }
    return best;
}

int main()
{
  
    // Read input. First line is number of datapoints. T
    int n;
    cin >> n; cin.ignore();
    // Then read data ponts in form
    // n1 t1 
    // n2 t2 ...
    double data[n][2];
    for (int i = 0; i < n; i++) {
        int num;
        int t;
        cin >> num >> t; cin.ignore();
        data[i][0] = num;
        data[i][1] = t;
    }

    string output;
    double best = -1;
  
    // Print best value of C and its corresponding error for each complexity (to stderr)
    for(int i=0; i<last;i++)
    {
        
        string name = get_string((complexity)i);
        double C = get_const(data,n,(complexity)i);
        double err = get_error(n,data, C,(complexity)i );
        if(err<best || best==-1)
        {
            best = err;
            output = name;
        }
        cerr << "------" + name + "-------" << endl;
        cerr << "C=" + to_string(C) << endl;
        cerr << "Error="+to_string(err) << endl;
    }
    // Send string version of best complexity to stdout
    cout << output << endl;
}
