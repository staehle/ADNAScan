
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
	    "AAAAACAATAACACGGCTGTTTSDAAAGCA"

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

		std::string CurrAdap = AdapterSearch.substr(startIndex, (endIndex)); // Looking at LAST part of the sequence
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
        std::cout << "\n\n\n No Adapters Found in First Half of sequence" << '\n';
    else{
        std::cout << " \n\n\n Longest Adapter : " << Adapters[RemoveAdapter] << '\n';
        std::cout << " Is found with length " << LongestAdapter.length() << " : " << LongestAdapter << "\n" << '\n';
        std::cout << " Adapter(s) were found in First Half of sequence " << '\n';
        for(int i = 0; i < NumbOfArrays.size(); i++) {
            std::cout << " Adapter." << NumbOfArrays[i] << " : " << Adapters[NumbOfArrays[i]] << '\n';
        }
    }

        std::cout << "\n\n\n" << '\n';





        counter = 0; // Counter for Adapters
        std::string LongestAdapter2 = ""; // Longest Adapter Found. - This is what SHOULD be Removed
        int RemoveAdapter2; // Longest Adapter that is found in sequence (The number in my array)
        std::vector< int > NumbOfArrays2; //Array for all the adapters found - For testing
        bool AdapterFound2 = false; // If Adapter Found




        while (counter < NumberOfAdapters){ //


        std::cout << "Adapter " << Adapters[counter] << " is being checked. \n" << '\n';


        std::string AdapterSearch = "";

        AdapterSearch.assign(Adapters[counter]);

    	int endIndex = AdapterSearch.length();


    	std::string FoundString = "";



    	int CurrentIncrementer = 0; // Incrementer for the substring Current Adapter

    	// Currently working on Second Half of Adapter

    	for (int startIndex = (str.length()- endIndex); startIndex <= (str.length()-MinLengthAdapters); startIndex = startIndex + 1)
    	{



    		int teststrlength = str.length();

    		//std::cout << "Length of string : " << teststrlength << " Length of Start Index : " << startIndex << '\n';


    		std::string CurrSeq = str.substr(startIndex, (startIndex - endIndex));

    		std::string CurrAdap = AdapterSearch.substr(0, (endIndex - CurrentIncrementer)); //Looking at FIRST part of Sequence
    		std::cout << CurrAdap << ' ' << CurrSeq << '\n';






    		std::size_t found = CurrSeq.find(CurrAdap);
    		if (found != std::string::npos) {
    			std::cout << "\nAdapter found at length: " << CurrSeq.length() << '\n';
    			FoundString.assign(CurrSeq);
    			NumbOfArrays2.push_back(counter);


    			if (AdapterFound2 == false){ // If first Adapter Found
    			    std::cout << "Longest Adapter is currently " << FoundString << '\n';
    			    LongestAdapter2.assign(FoundString);
    			    AdapterFound2 = true;
    			    RemoveAdapter2 = counter; // assigning adapter to remove
    			    break;
    			}
    			else{ // If another adapter is found
    			    if(FoundString.length() >= LongestAdapter2.length()){
    			        LongestAdapter2.assign(FoundString);
    			        std::cout << "New Longest Adapter is currently " << FoundString << '\n';
    			        RemoveAdapter2 = counter;
    			        break;}
    			    else {
    			        std::cout << "Adapter Found, but not longer than current Longest Adapter " << FoundString <<  " vs " << LongestAdapter <<'\n';
    			        break;}
    			    }

    			}







    		CurrentIncrementer++;


    	}

        std::cout << "\nAdapter " << Adapters[counter] << " is searched. \n\n" << '\n';

        // Counter for Adapters
        counter ++;


        }

        // Print Statements

        if (AdapterFound2 == false)
            std::cout << "\n\n\n No Adapters Found in Second Half of sequence" << '\n';
        else {
            std::cout << " \n\n\n Longest Adapter : " << Adapters[RemoveAdapter2] << '\n';
            std::cout << " Is found with length " << LongestAdapter2.length() << " : " << LongestAdapter2 << "\n" << '\n';
            std::cout << " Adapter(s) were found in Second Half of sequence " << '\n';
            for(int i = 0; i < NumbOfArrays2.size(); i++) {
                std::cout << " Adapter." << NumbOfArrays2[i] << " : " << Adapters[NumbOfArrays2[i]] << '\n';
            }
    }



	return 0;
}


// Need to Add the real Adapters
// Get Sequence in from Other Code

// Remove Adapters into our new File
