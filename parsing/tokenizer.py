import subprocess
import json

def tokenize(code):
    """Simple tokenization for Gelang language"""
    # This is a very basic tokenizer and will need to be more sophisticated
    keywords = [
        'PROGRAM', 'IS', 'BEGIN', 'END', 'IF', 'THEN', 'ELSE', 'ENDIF', 
        'WHILE', 'DO', 'ENDWHILE', 'REPEAT', 'UNTIL', 'FOR', 'FROM', 
        'TO', 'DOWNTO', 'ENDFOR', 'PROCEDURE', 'READ', 'WRITE'
    ]
    
    # Basic tokenization
    tokens = []
    current_token = ''
    for char in code:
        if char.isspace():
            if current_token:
                tokens.append(current_token)
                current_token = ''
            continue
        
        # Check for operators and punctuation
        if char in [':','=','+','-','*','/','%','(',')',';','[',']',',']:
            if current_token:
                tokens.append(current_token)
                current_token = ''
            tokens.append(char)
        else:
            current_token += char
    
    # Add last token if exists
    if current_token:
        tokens.append(current_token)
    
    return tokens

def parse_gelang(code):
    """Parse Gelang code using C++ parser"""
    # Tokenize the input
    tokens = tokenize(code)
    
    # Write tokens to a temporary file
    with open('temp_tokens.txt', 'w') as f:
        f.write('\n'.join(tokens))
    
    # Run C++ parser
    subprocess.run(['./gelang_parser', 'temp_tokens.txt', 'output.json'], 
                   check=True)
    
    # Read parsed JSON
    with open('output.json', 'r') as f:
        return json.load(f)

# Example usage
code = """
PROGRAM IS 
    BEGIN
        x := 5;
        y := x + 3;
    END
"""

try:
    parsed_ast = parse_gelang(code)
    print(json.dumps(parsed_ast, indent=2))
except Exception as e:
    print(f"Parsing error: {e}")