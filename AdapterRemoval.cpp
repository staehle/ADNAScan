
//https://github.com/jhhung/PEAT

//https://github.com/jhhung/PEAT/releases
//  - Download for working Trimmer


//Curr Working Adapter Trimmer


// Current code



#include <iostream>
#include <string>
#include <vector>

// So far just recognizing the adapters found in the Sequence
// Many Print statements and Extra Information for Testing Purpose

int main()
{
    
    const int MinLengthAdapters = 6; // Min Length for accepted Adapters
    const int NumberOfAdapters = 7; // Amount of Adapters we are using
    
    
	std::string str = "AGATCGGAAGAGCACACGTCTGAACTCCAGTCACCAGATCATCTCGTATGCNGTCTTCTGNNTGAAAAAAAAAAACAAATACATAGTATACTTTCTTTATGAAATAAAAAATAAAAACAATAACA";
	// (this is the String from Sequence) - this string should be given from other code (After Overlap)
	
	
	// Array of the adapters - This is not the real Adapters, made them for testing purpose
	std::string Adapters[] = 
	{
	    "AGATCGAGTTTCGCGCCGCGGGCAGATCGG",
	    "TTTCCCCATCTTCGCCGTCCTCCTGCTTGC",
	    "CTTGATCACCGGGTCGCCGTTCTTCGGCGG",
	    "AGATCGGAASDAGAGCAGATCGGAAGAGCA",
	    "CTGGAATCGCGGACACGACCCGATCTCATT",
	    "TGCCGGAGTTTCGCGCCGCGGSSSAGATCG",
	    "CCCGCGGCAAACTCGGCTGTTTSDAAAGCA"
	    
	};

  

    std::string LongestAdapter = ""; // Longest Adapter Found. - This is what SHOULD be Removed


    
    int counter = 0; // Counter for Adapters
    int RemoveAdapter; // Longest Adapter that is found in sequence (The number in my array)
    std::vector< int > NumbOfArrays; //Array for all the adapters found - For testing
    
    bool AdapterFound = false; // If Adapter Found
    
    while (counter < NumberOfAdapters){ //
        
        
    std::cout << "Adapter " << Adapters[counter] << " is being checked. \n" << '\n';


    std::string AdapterSearch = "";

    AdapterSearch.assign(Adapters[counter]);

	int endIndex = AdapterSearch.length();

	
	std::string FoundString = "";

	for (int startIndex = 0; startIndex <= (endIndex - MinLengthAdapters); startIndex = startIndex + 1)
	{


		std::string CurrSeq = str.substr(0, (endIndex - startIndex));

		std::string CurrAdap = AdapterSearch.substr(startIndex, (endIndex));
		std::cout << CurrAdap << ' ' << CurrSeq << '\n';



		std::size_t found = CurrSeq.find(CurrAdap);
		if (found != std::string::npos) {
			std::cout << "\nAdapter found at length: " << CurrSeq.length() << '\n';
			FoundString.assign(CurrSeq);
			NumbOfArrays.push_back(counter);
			
			
			if (AdapterFound == false){ // If first Adapter Found
			    std::cout << "Longest Adapter is currently " << FoundString << '\n';
			    LongestAdapter.assign(FoundString);
			    AdapterFound = true;
			    RemoveAdapter = counter; // assigning adapter to remove
			    break;
			}
			else{ // If another adapter is found
			    if(FoundString.length() >= LongestAdapter.length()){
			        LongestAdapter.assign(FoundString);
			        std::cout << "New Longest Adapter is currently " << FoundString << '\n';
			        RemoveAdapter = counter;
			        break;}
			    else {
			        std::cout << "Adapter Found, but not longer than current Longest Adapter " << FoundString <<  " vs " << LongestAdapter <<'\n';
			        break;}
			    }
			    
			}

		



	}

    std::cout << "\nAdapter " << Adapters[counter] << " is searched. \n\n" << '\n';
    
    // Counter for Adapters
    counter ++;
    }
    
    // Print Statements
    
    if (AdapterFound == false)
        std::cout << "\n\n\n No Adapters Found" << '\n';
    else
        std::cout << " \n\n\n Longest Adapter : " << Adapters[RemoveAdapter] << '\n';
        std::cout << " Is found with length " << LongestAdapter.length() << " : " << LongestAdapter << "\n" << '\n';
        std::cout << " Adapter(s) were found in sequence " << '\n';
        for(int i = 0; i < NumbOfArrays.size(); i++) {
            std::cout << i+1 << ". " << NumbOfArrays[i] << " : " << Adapters[NumbOfArrays[i]] << '\n';
}
	return 0;
}


// Need to Add the real Adapters 
// Get Sequence in from Other Code

// Remove Adapters into our new File
