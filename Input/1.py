import os
import pandas as pd

input_dir = r"c:\Users\ADMIN\Downloads\KTLT\Đồ án\Input"

for filename in os.listdir(input_dir):
    if filename.endswith(".csv"):
        filepath = os.path.join(input_dir, filename)
        try:
            df = pd.read_csv(filepath)
            
            # Check for 'Name' or 'ticker'
            name_col = 'Name' if 'Name' in df.columns else None
            
            # If it's already 'ticker', we don't need to rename, just select.
            if name_col is None and 'ticker' in df.columns:
                name_col = 'ticker'
                
            if name_col and 'close' in df.columns and 'volume' in df.columns and 'date' in df.columns:
                df = df[['close', name_col, 'volume', 'date']]
                if name_col == 'Name':
                    df = df.rename(columns={'Name': 'ticker'})
                
                df.to_csv(filepath, index=False)
                print(f"Processed: {filename}")
            else:
                print(f"Skipped {filename}: Missing one or more required columns.")
                
        except Exception as e:
            print(f"Error processing {filename}: {e}")
