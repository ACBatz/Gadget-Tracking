//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  CODE FILENAME: Batzel-assn1-prog.cpp
//  DESCRIPTION:   This program compares gadgets being purchased against ones that are to be sold. A file must be included in the command line that contains the
//                 gadgets being purchased and sold. If a file is included in the command line and it exists, a stack linked list and a queue linked list will be
//                 created and initialized to empty. As the file is read the data will be processed until all data has been read or there is not enough memory
//                 to allocate further nodes to either linked list. Purchased items will be stored into the stack and sold items will be stored into a queue. After
//                 all data is read, gadgets that are sold be compared against gadgets in inventory, ones that were purchased and stored in a stack. The program
//                 will display the results of how many gadgets were sold at a particular price, 45% more than the price purchased. If there aren't enough gadgets
//                 in inventory to support the sale, the program will state how many gadgets are short. Or if there are more gadgets in inventory than there were
//                 sold, the program will state how many remaining gadgets there are in the inventory.
//  CLASS/TERM:    CS372 / 14S8W2
//  DESIGNER:      Andrew C. Batzel
//  FUNCTIONS:     main- reads text file from command line, makes calls to create stack and queue and process data from file.
//                 createEmptyStack- creates an empty stack to store purchased data.
//                 createEmptyQueue- creates and empty queue to store sales data.
//                 readFile- reads file provided on the command line and returns true if the file exists.
//                 processFileData- reads data from file and uses a switch to call functions to push or enqueue the data.
//                 pushStack- adds a node to the top of the linked list stack.
//                 enQueue- adds a node to the back of the linked list queue.
//                 isEmptyStack- tests if the stack is empty.
//                 isEmptyQueue- tests if the queue is empty.
//                 isFullStack- tests if the stack is full.
//                 isFullQueue- tests if the queue is full.
//                 processSalesData- calls stack and queue functions to compare sales data and purchased data.
//                 popStack- removes the top node from the stack, extracting the data from it.
//                 deQueue- removes the front node from the queue, extracting the data from it.
//                 destroyLists- deletes the stack and queue linked lists from the heap.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#include<iostream>
#include<iomanip>
#include<fstream>
#include<cstddef>

using namespace std;

// constants
const float MARKUP = 1.45;                      // percentage of price increase for selling a gadget
const int MAX_ARGC = 1;                         // maximum input agruments allowed
struct purchaseRecords {
                        int quantityPurchased;
                        float costPerGadget;
                        purchaseRecords *nextPurchase;
                       };
struct salesRecords {
                     int quantitySold;
                     int invoiceNum;
                     salesRecords *nextSale;
                    };
struct purchaseStack {
                      int count;
                      purchaseRecords *stackTop;
                     };
struct salesQueue {
                   int count;
                   salesRecords *queueFront;
                   salesRecords *queueBack;
                  };

// function prototypes
void createEmptyStack(purchaseStack *&mainStack);
void createEmptyQueue(salesQueue *&mainQueue);
bool readFile(string fileName, ifstream& dataIn);
void processFileData(ifstream& dataIn, purchaseStack *&mainStack, salesQueue *&mainQueue);
void pushStack(purchaseStack *&mainStack, int&, float&, bool);
void enQueue(salesQueue *&mainQueue, int&, int&);
bool isEmptyStack(purchaseStack *&mainStack);
bool isEmptyQueue(salesQueue *&mainQueue);
bool isFullStack(purchaseStack *&mainStack);
bool isFullQueue(salesQueue *&mainQueue);
void processSalesData(purchaseStack *&mainStack, salesQueue *&mainQueue);
void popStack(purchaseStack *&mainStack, int&, float&);
void deQueue(salesQueue *&mainQueue, int&, int&);
void destroyLists(purchaseStack *&mainStack, salesQueue *&mainQueue);

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     main
//  DESCRIPTION:  reads text file from command line, makes calls to create stack and queue and process data from file.
//  INPUT:
//     Parameter:   argc
//                  argv
//  OUTPUT:
//       Return Val:   Returns the value 0 upon sucessful completion of the program.
//  CALLS TO:     readFile
//                createEmptyStack
//                createEmptyQueue
//                processFileData
//                isFullStack
//                isFullQueue
//                processSalesData
//                destroyLists
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    ifstream dataIn;            
    string fileName;
    bool fileExists;
    purchaseRecords *stack;
    purchaseStack *mainStack;
    salesQueue *mainQueue;
    salesRecords *queue;
  
    if (argc < MAX_ARGC + 1)
    {
       cout << "ERROR - must enter an input file as a command line argument." << endl
            << "Restart the program after entering a file name." << endl << endl;
    } // end if a file is not in the command line
    else
    {        
        fileName = argv[argc - MAX_ARGC];
        fileExists = readFile(fileName, dataIn);
        // diplay error if the input file doesn't exist
        if (!fileExists)
           cout << "ERROR - Command line file arguement does not exist." << endl << endl;
        else
        {
            createEmptyStack(mainStack);
            
            createEmptyQueue(mainQueue);
            
            processFileData(dataIn, mainStack, mainQueue);
            // if no memory heap errors occured
            if (!isFullStack(mainStack) || !isFullQueue(mainQueue))
               processSalesData(mainStack, mainQueue);
            
            destroyLists(mainStack, mainQueue);     
        } // end else input files exists  
    } // end else a file was in the command line
    
    system("pause");
    return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     createEmptyStack
//  DESCRIPTION:  creates an empty stack to store purchased data.
//  INPUT:
//     Parameter:   mainStack
//  OUTPUT:
//     Parameter:   mainStack
//  CALLS TO:     none
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void createEmptyStack(purchaseStack *&mainStack)
{    
     mainStack = new (nothrow) purchaseStack;
     
     mainStack->count = 0;
     mainStack->stackTop = NULL;
     
     return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     createEmptyQueue
//  DESCRIPTION:  creates an empty queue to store sales data.
//  INPUT:
//     Parameter:   mainQueue
//  OUTPUT:
//     Parameter:   mainQueue
//  CALLS TO:     none
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void createEmptyQueue(salesQueue *&mainQueue)
{   
     mainQueue = new(nothrow) salesQueue;
     
     mainQueue->count = 0;
     mainQueue->queueFront = NULL;
     mainQueue->queueBack = NULL;
     
     return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     readFile
//  DESCRIPTION:  reads file provided on the command line and returns true if the file exists.
//  INPUT:
//     Parameter:   fileName
//                  dataIn
//  OUTPUT:
//     Parameter:   dataIn
//     Return Val:  fileExists
//  CALLS TO:     none
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

bool readFile(string fileName, ifstream& dataIn)
{
     bool fileExists;
     
     dataIn.open(fileName.c_str());
     
     dataIn ? fileExists = true: fileExists = false;
     
     return fileExists;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     processFileData
//  DESCRIPTION:  reads data from file and uses a switch to call functions to push or enqueue the data.
//  INPUT:
//     Parameter:   dataIn
//                  mainStack
//                  mainQueue
//  OUTPUT:
//     Parameter:   dataIn
//                  mainStack
//                  mainQueue
//  CALLS TO:     isFullStack
//                isFullQueue
//                pushStack
//                enQueue
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void processFileData(ifstream& dataIn, purchaseStack *&mainStack, salesQueue *&mainQueue)
{
     char processType;
     int tempQuantityPurchased,
         tempQuantitySold,
         tempInvoiceNum;
     float tempCostPerGadget;
     
     dataIn >> processType;
     
     while (dataIn && !isFullStack(mainStack) && !isFullQueue(mainQueue))
     {
           switch(processType)
           {
              case 'P':       // data type is a purchase
                   dataIn >> tempQuantityPurchased;
                   dataIn >> tempCostPerGadget;
                   pushStack(mainStack, tempQuantityPurchased, tempCostPerGadget, true);
                   break;
              case 'S':       // data type is a sale
                   dataIn >> tempQuantitySold;
                   dataIn >> tempInvoiceNum;
                   enQueue(mainQueue, tempInvoiceNum, tempQuantitySold);
                   break;
           } // end switch of purchases or sales
           dataIn >> processType;
     } // end while there is still data in file and no memory heap errors have occured
     dataIn.close();
     return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     pushStack
//  DESCRIPTION:  adds a node to the top of the linked list stack.
//  INPUT:
//     Parameter:   mainStack
//                  quantityPurchased
//                  costPerGadget
//                  display
//  OUTPUT:
//     Parameter:   mainStack
//                  quantityPurchased
//                  costPerGadget
//  CALLS TO:     isEmptyStack
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void pushStack(purchaseStack *&mainStack, int& quantityPurchased, float& costPerGadget, bool display)
{
     purchaseRecords *newNode;
    
     newNode = new (nothrow) purchaseRecords;

     newNode->quantityPurchased = quantityPurchased;
     newNode->costPerGadget = costPerGadget;
     // next node pointer is NULL when a new node is added to an empty list
     if (isEmptyStack(mainStack))
        newNode->nextPurchase = NULL;
     // link the next node pointer to the top of the stack  
     else
         newNode->nextPurchase = mainStack->stackTop;
         
     mainStack->stackTop = newNode;
     mainStack->count++;
     // when file is read will output purchased data
     if (display)
        cout << right << setw(4) << newNode->quantityPurchased << left << " gadgets recieved at $" << fixed << showpoint << setprecision(2) << newNode->costPerGadget << " each - Total Cost: $" << right << setw(10) << newNode->quantityPurchased * newNode->costPerGadget << endl;
     // no data will be displayed when pushStack is called from processSalesData
     return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     enQueue
//  DESCRIPTION:  adds a node to the back of the linked list queue.
//  INPUT:
//     Parameter:   mainQueue
//                  invoiceNum
//                  quantitySold
//  OUTPUT:
//     Parameter:   mainQueue
//                  invoiceNum
//                  quantitySold
//  CALLS TO:     isEmptyQueue
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void enQueue(salesQueue *&mainQueue, int& invoiceNum, int& quantitySold)
{
     salesRecords *newNode;
   
     newNode = new (nothrow) salesRecords;
     
     newNode->quantitySold = quantitySold;
     newNode->invoiceNum = invoiceNum;
     newNode->nextSale = NULL;
     // the pointers to the front and back of the queue are equal when a new node is added to an empty queue
     if (isEmptyQueue(mainQueue))
     {
        mainQueue->queueFront = newNode;
        mainQueue->queueBack = newNode;
     } // end if the queue is empty
     // link the back node next pointer to the new node and make the new node the back of the queue
     else
     {
         mainQueue->queueBack->nextSale = newNode;
         mainQueue->queueBack = newNode;
     } // end else the queue is not empty
     mainQueue->count++;
    
     return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     isEmptyStack
//  DESCRIPTION:  tests if the stack is empty.
//  INPUT:
//     Parameter:   mainStack
//  OUTPUT:
//     Parameter:   mainStack
//     Return Val:  empty
//  CALLS TO:     none
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

bool isEmptyStack(purchaseStack *&mainStack)
{
     bool empty;
     
     mainStack->count == 0 ? empty = true: empty = false;
     
     return empty;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     isEmptyQueue
//  DESCRIPTION:  tests if the queue is empty.
//  INPUT:
//     Parameter:   mainQueue
//  OUTPUT:
//     Parameter:   mainQueue
//     Return Val:  empty
//  CALLS TO:     none
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

bool isEmptyQueue(salesQueue *&mainQueue)
{
     bool empty;
     
     mainQueue->count == 0 ? empty = true: empty = false;
     
     return empty;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     isFullStack
//  DESCRIPTION:  tests if the stack is full.
//  INPUT:
//     Parameter:   mainStack
//  OUTPUT:
//     Parameter:   mainStack
//     Return Val:  noHeap
//  CALLS TO:     none
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

bool isFullStack(purchaseStack *&mainStack)
{
     bool noHeap;
     purchaseRecords *newNode;
     
     newNode = new (nothrow) purchaseRecords;
     // the heap if full when a new node cannot be allocated
     if (newNode == NULL)
         noHeap = true;

     else
         noHeap = false;
         
     delete newNode;
     
     return noHeap;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     isFullQueue
//  DESCRIPTION:  tests if the queue is full.
//  INPUT:
//     Parameter:   mainQueue
//  OUTPUT:
//     Parameter:   mainQueue
//     Return Val:  noHeap
//  CALLS TO:     none
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

bool isFullQueue(salesQueue *&mainQueue)
{
     bool noHeap;
     salesRecords *newNode;
     
     newNode = new (nothrow) salesRecords;
     // the heap if full when a new node cannot be allocated
     if (newNode == NULL)
         noHeap = true;

     else
         noHeap = false;
         
     delete newNode;

     
     return noHeap;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     processSalesData
//  DESCRIPTION:  calls stack and queue functions to compare sales data and purchased data.
//  INPUT:
//     Parameter:   mainStack
//                  mainQueue
//  OUTPUT:
//     Parameter:   mainStack
//                  mainQueue
//  CALLS TO:     isEmptyQueue
//                isEmptyStack
//                deQueue
//                popStack
//                pushStack
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void processSalesData(purchaseStack *&mainStack, salesQueue *&mainQueue)
{
     int quantityPurchased,
         quantitySold,
         quantityRemainder,
         invoiceNum,
         totalQuantitySold,
         invoiceDeficit;
     float purchaseCostPerGadget,
           totalSaleCost;
     bool multiplePushStack,
          multiplePopStack;
     
     while (!isEmptyQueue(mainQueue) && !isEmptyStack(mainStack))
     {
         deQueue(mainQueue, invoiceNum, quantitySold);
         multiplePushStack = false;
         totalQuantitySold = 0;
         totalSaleCost = 0;
         
         cout << "\nSold on invoice " << invoiceNum << ":" << endl;
         
         while (!isEmptyStack(mainStack) && (quantitySold != 0))
         {
               // get data from the purchase stack
               popStack(mainStack, quantityPurchased, purchaseCostPerGadget);
               
               if (quantityPurchased >= quantitySold)
               {
                  // store the remaining gadgets in a seperate variable
                  quantityRemainder = quantityPurchased - quantitySold;
                  // push the remaining gadgets back into the stack
                  if (quantityRemainder != 0)
                     pushStack(mainStack, quantityRemainder, purchaseCostPerGadget, false);
                  
                  cout << setw(7) << right << quantitySold << left << " gadgets sold at $" << purchaseCostPerGadget * MARKUP << " each - Total recieved: $" << setw(7) << setprecision(2)
                       << right << purchaseCostPerGadget * MARKUP * quantitySold << endl;
                  // update the number of gadgets sold
                  totalQuantitySold += quantitySold;
                  // update the total cost for gadgets for the current invoice
                  totalSaleCost += purchaseCostPerGadget * MARKUP * quantitySold;
                  // set quantitySold to zero to end the loop for current invoice
                  quantitySold = 0;
               } // end if there are more gadgets in the purchase stack then the number needed to complete the invoice sale
               else
               {
                   // more than one push function was used, display lines to calculate the total amount of gadgets and total cost from multiple nodes
                   if (!isEmptyStack(mainStack))
                      multiplePushStack = true;
                   
                   cout << setw(7) << right << quantityPurchased << left << " gadgets sold at $" << purchaseCostPerGadget * MARKUP << " each - Total recieved: $" << setw(7) << setprecision(2)
                        << right << purchaseCostPerGadget * MARKUP * quantityPurchased << endl;
                   // update the number of gadgets sold
                   totalQuantitySold += quantityPurchased;
                   // update the total cost for gadgets for the current invoice
                   totalSaleCost += purchaseCostPerGadget * MARKUP * quantityPurchased;
                   // update the number of gadgets still needed to complete the invoice
                   quantitySold -= quantityPurchased;
               } // end else there are more gadgets needed in the invoice then there are in the current stack node
         } // end while the stack is not empty and invoice sales is completed
         if (multiplePushStack)
         {
            cout << "  -----" << setw(46) << "" << "--------" << endl
                 << setw(7) << right << totalQuantitySold << left << " gadgets" << setw(37) << right << "TOTAL SALE" << setw(2) << "$" << setw(7) << setprecision(2) << totalSaleCost << endl;
         } // end if multiple push functions were called
         // when there aren't enough gadgets to support sales
         if (isEmptyStack(mainStack) && (quantitySold != 0) )
            cout << endl << setw(7) << right << quantitySold << " gadgets SHORT on invoice " << invoiceNum << endl;
     } // end while the stack and queue aren't empty
     
     if (quantitySold > 0)
     {
        cout << "\nSales orders that could not be filled:" << endl;
        cout << setw(7) << right << quantitySold << left << " gadgets SHORT on invoice " << invoiceNum << endl;
        while (!isEmptyQueue(mainQueue))
        {
              deQueue(mainQueue, invoiceNum, quantitySold);
              cout << setw(7) << right << quantitySold << left << " gadgets SHORT on invoice " << invoiceNum << endl;
        } // end while the queue is not empty
     } // end if there is a gadget deficit for invoices
     
     else if (!isEmptyStack(mainStack))
     {
        quantityRemainder = 0;
        cout << "\nRemaining Inventory:" << endl << endl;
        while (!isEmptyStack(mainStack))
        {
              popStack(mainStack, quantityPurchased, purchaseCostPerGadget);
              cout << setw(4) << right << quantityPurchased << left << " gadgets recieved at $" << setprecision(2) << purchaseCostPerGadget << " each" << endl;
              quantityRemainder += quantityPurchased;
        } // end while the stack is not empty
        cout << "----" << endl
             << setw(4) << right << quantityRemainder << left << " gadgets remain in the inventory" << endl;
     } // end if there are gadgets left in inventory
     // if amount of gadgets sold is equal to the amount in inventory
     else
        cout << "\nNo remaining inventory" << endl;
     
     return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     popStack
//  DESCRIPTION:  removes the top node from the stack, extracting the data from it.
//  INPUT:
//     Parameter:   mainStack
//                  quantityPurchased
//                  costPerGadget
//  OUTPUT:
//     Parameter:   mainStack
//                  quantityPurchased
//                  costPerGadget
//  CALLS TO:     isEmptyStack
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void popStack(purchaseStack *&mainStack, int& quantityPurchased, float& costPerGadget)
{     
     if (!isEmptyStack(mainStack))
     {
        quantityPurchased = mainStack->stackTop->quantityPurchased;
        costPerGadget = mainStack->stackTop->costPerGadget;

        mainStack->stackTop = mainStack->stackTop->nextPurchase;
        
        mainStack->count--;
     } // end if the stack is not empty
     
     return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     deQueue
//  DESCRIPTION:  removes the front node from the queue, extracting the data from it.
//  INPUT:
//     Parameter:   mainQueue
//                  deInvoiceNum
//                  deQuantitySold
//  OUTPUT:
//     Parameter:   mainQueue
//                  deInvoiceNum
//                  deQuantitySold
//  CALLS TO:     isEmptyQueue
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void deQueue(salesQueue *&mainQueue, int& deInvoiceNum, int& deQuantitySold)
{
     if (!isEmptyQueue(mainQueue))
     {
         deInvoiceNum = mainQueue->queueFront->invoiceNum;
         deQuantitySold = mainQueue->queueFront->quantitySold;

         mainQueue->queueFront = mainQueue->queueFront->nextSale;
         
         mainQueue->count--;
     } // end if the queue is not empty
     return;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//  FUNCTION:     destroyLists
//  DESCRIPTION:  deletes the stack and queue linked lists from the heap.
//  INPUT:
//     Parameter:   mainStack
//                  mainQueue
//  OUTPUT:
//     Parameter:   mainStack
//                  mainQueue
//  CALLS TO:     none
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void destroyLists(purchaseStack *&mainStack, salesQueue *&mainQueue)
{
     while (mainStack->stackTop != NULL)
     {
           mainStack->stackTop = mainStack->stackTop->nextPurchase;
     } // end while there is data in top of the stack
     delete mainStack;
     
     while (mainQueue->queueFront != NULL)
     {
           mainQueue->queueFront = mainQueue->queueFront->nextSale;
     } // end while there is data in the front of the queue
     delete mainQueue;
     
     return;
}
