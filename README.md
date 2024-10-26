# Essay Search

# Text Search Tool

This tool reads a set of text files (essays) from a specified folder and searches for queries listed in a separate text file. The results are saved to an output file with the specified name.

## Input

1. **Folder of Text Files**: A folder containing text files (named `0.txt`, `1.txt`, etc.).
2. **Query File**: A text file containing search queries.
3. **Output File Name**: The name of the file where results will be saved.

## Output

The tool generates a text file with the given output name, containing the results of the search based on the queries provided.

## Usage

1. Place all essay files in a folder.
2. Prepare a text file with your search queries (one query per line).
3. Run the script, providing the folder path, query file, and desired output file name.
4. The output file will be generated in the specified location.

## Example

Run:
```bash
./essay-search data query.txt output.txt
```