import paramiko
import csv
from getpass import getpass
# SSH Connection details
ssh_host = 'ipnp30.troja.mff.cuni.cz'
ssh_username = 'mbiros'
private_key_path = 'my_key'
remote_file_path = 'file.csv'

# Function to add a row to a CSV file on the remote machine
def add_row_to_csv(data):
    # Establish SSH connection with private key authentication
    ssh_client = paramiko.SSHClient()
    ssh_client.set_missing_host_key_policy(paramiko.AutoAddPolicy())

    # Load private key
    passphrase = getpass("Enter passphrase for private key: ")
    private_key = paramiko.RSAKey.from_private_key_file(private_key_path, password=passphrase)
    del passphrase

    # Connect with private key authentication
    ssh_client.connect(hostname=ssh_host, username=ssh_username, pkey=private_key)

    # Read existing data
    sftp_client = ssh_client.open_sftp()
    
    # List files in the remote directory
    file_list = sftp_client.listdir(".")
    print(" Printing files in the remote directory to demonstrate functionality:")
    for file_name in file_list:
        if file_name[0] != ".": print("  ",file_name)

    try:
        with sftp_client.file(remote_file_path, 'r') as remote_file:
            csv_reader = csv.reader(remote_file)
            rows = list(csv_reader)
    except:
        print(remote_file_path, "doesnt exist")
    # Add new row
    rows.append(data)

    # Write updated data back to the CSV file
    with sftp_client.file(remote_file_path, 'w') as remote_file:
        csv_writer = csv.writer(remote_file)
        csv_writer.writerows(rows)

    # Close connections
    sftp_client.close()
    ssh_client.close()

# Example data for the new row
new_row_data = ['New', 'Data', 'Goes', 'Here']

# Call the function to add a row to the CSV
add_row_to_csv(new_row_data)
