import scipy.io
import matplotlib.pyplot as plt
import numpy as np
import os

TitleSize = 40
FontSize = 30
LineWidth = 5

# See this link to make sense of the data: 
# https://obsidian.aatif.no/NTNU/6.-Semester/Optimalisering-og-regulering/Lab/

def load_mat_file(file_path: str) -> dict:
    # Load the MATLAB file
    mat_data = scipy.io.loadmat(file_path)
    return mat_data

def get_file_path(LabDay: int) -> str:
    print("")
    files = [f for f in os.listdir('data') if f.startswith(f'LabDay{LabDay}')]
    for i, file in enumerate(files):
        print(f"{i+1}. {file}")
    choice = int(input("Choose a file to plot: ")) - 1
    file_path = os.path.join('data', files[choice])
    return file_path

def load_file_data(file_path: str) -> dict:
    
    raw_data_dict = load_mat_file(file_path)
    raw_data = raw_data_dict['all_data']

    data = dict()
    data['time'] = raw_data[0]
    data['travel'] = raw_data[1]
    data['travel_rate'] = raw_data[2]
    data['pitch'] = raw_data[3]
    data['pitch_rate'] = raw_data[4]
    data['elevation'] = raw_data[5]
    data['elevation_rate'] = raw_data[6]
    data['pitch_reference'] = np.degrees(raw_data[7])
    data['elevation_reference'] = np.degrees(raw_data[8])
    data['v_d'] = raw_data[9]
    data['v_s'] = raw_data[10]

    print(len(raw_data)) #day2,3 has 15 elements, day4 has 18 elements
    if len(raw_data) == 18:
        data['x_star'] = [np.degrees(x) for x in raw_data[11:17]]
        data['constraint'] = raw_data[17]
    else:
        data['x_star'] = [np.degrees(x) for x in raw_data[11:15]]


    return data


def plot_lab_day2():
   
    print("")
    print("Plot for different q-values")
    print("1. Travel and Pitch vs Optimal Travel and Pitch")
    print("2. How the input changes with different q-values")
    choice = int(input("Enter your choice: ")) 

    if choice == 1:
        file_path = get_file_path(2)
        data = load_file_data(file_path)

        header = input("Enter the header for the q-values: ")
        optimal_travel, optimal_travel_rate, optimal_pitch, optimal_pitch_rate = data['x_star'][0], data['x_star'][1], data['x_star'][2], data['x_star'][3]
        travel, travel_rate, pitch, pitch_rate = data['travel']+180, data['travel_rate'], data['pitch'], data['pitch_rate']
        time = data['time'].squeeze()

        # Plotting
        fig, axs = plt.subplots(4, 1, figsize=(12, 24))  # Creating 4 subplots

        # Each subplot for each pair of variables
        axs[0].plot(time, travel.squeeze(), label='Actual Travel', linewidth=LineWidth)
        axs[0].plot(time, optimal_travel.squeeze(), label='Optimal Travel', linestyle='--', linewidth=LineWidth)
        axs[0].set_title('Travel vs Optimal Travel', fontsize=TitleSize)
        axs[0].set_xlabel('Time (s)', fontsize=FontSize)
        axs[0].set_ylabel('Travel (deg)', fontsize=FontSize)
        axs[0].legend(fontsize=FontSize)
        axs[0].grid(True)
        axs[0].tick_params(axis='both', which='major', labelsize=FontSize)

        axs[1].plot(time, travel_rate.squeeze(), label='Actual Travel Rate', linewidth=LineWidth)
        axs[1].plot(time, optimal_travel_rate.squeeze(), label='Optimal Travel Rate', linestyle='--', linewidth=LineWidth)
        axs[1].set_title('Travel Rate vs Optimal Travel Rate', fontsize=TitleSize)
        axs[1].set_xlabel('Time (s)', fontsize=FontSize)
        axs[1].set_ylabel('Travel Rate (deg)', fontsize=FontSize)
        axs[1].legend(fontsize=FontSize)
        axs[1].grid(True)
        axs[1].tick_params(axis='both', which='major', labelsize=FontSize)

        axs[2].plot(time, pitch.squeeze(), label='Actual Pitch', linewidth=LineWidth)
        axs[2].plot(time, optimal_pitch.squeeze(), label='Optimal Pitch', linestyle='--', linewidth=LineWidth)
        axs[2].set_title('Pitch vs Optimal Pitch', fontsize=TitleSize)
        axs[2].set_xlabel('Time (s)', fontsize=FontSize)
        axs[2].set_ylabel('Pitch (deg)', fontsize=FontSize)
        axs[2].legend(fontsize=FontSize)
        axs[2].grid(True)
        axs[2].tick_params(axis='both', which='major', labelsize=FontSize)

        axs[3].plot(time, pitch_rate.squeeze(), label='Actual Pitch Rate', linewidth=LineWidth)
        axs[3].plot(time, optimal_pitch_rate.squeeze(), label='Optimal Pitch Rate', linestyle='--', linewidth=LineWidth)
        axs[3].set_title('Pitch Rate vs Optimal Pitch Rate', fontsize=TitleSize)
        axs[3].set_xlabel('Time (s)', fontsize=FontSize)
        axs[3].set_ylabel('Pitch Rate (deg)', fontsize=FontSize)
        axs[3].legend(fontsize=FontSize)
        axs[3].grid(True)
        axs[3].tick_params(axis='both', which='major', labelsize=FontSize)

        plt.tight_layout()
        plt.subplots_adjust(hspace=0.6)
        plt.show()
    elif choice == 2:
        # Load data for three different q-values
        data_q_equals_0_point_12 = load_file_data("data/LabDay2_q_equals_0_point_12.mat")
        data_q_equals_1_point_12 = load_file_data("data/LabDay2_q_equals_1_point_2.mat")
        data_q_equals_12 = load_file_data("data/LabDay2_q_equals_12.mat")

        # Extract time and pitch reference data for each q-value dataset
        time_0_12 = data_q_equals_0_point_12['time'].squeeze()
        pitch_0_12 = data_q_equals_0_point_12['pitch_reference'].squeeze()
        time_1_12 = data_q_equals_1_point_12['time'].squeeze()
        pitch_1_12 = data_q_equals_1_point_12['pitch_reference'].squeeze()
        time_12 = data_q_equals_12['time'].squeeze()
        pitch_12 = data_q_equals_12['pitch_reference'].squeeze()

        # Limit data to the first 20 seconds
        mask_0_12 = time_0_12 <= 20
        mask_1_12 = time_1_12 <= 20
        mask_12 = time_12 <= 20

        # Plotting
        fig, ax = plt.subplots(figsize=(12, 6))  # Creating a subplot for pitch comparison

        # Plotting pitch data with dotted lines
        ax.plot(time_0_12[mask_0_12], pitch_0_12[mask_0_12], 'r--', label='q = 0.12', linewidth=LineWidth)
        ax.plot(time_1_12[mask_1_12], pitch_1_12[mask_1_12], 'g--', label='q = 1.2', linewidth=LineWidth)
        ax.plot(time_12[mask_12], pitch_12[mask_12], 'b--', label='q = 12', linewidth=LineWidth)

        # Setting plot titles and labels
        ax.set_title('Pitch Input for Different q-values', fontsize=TitleSize)
        ax.set_xlabel('Time (s)', fontsize=FontSize)
        ax.set_ylabel('Pitch (deg)', fontsize=FontSize)
        ax.legend(fontsize=FontSize)
        ax.grid(True)
        ax.tick_params(axis='both', which='major', labelsize=FontSize)

        plt.tight_layout()
        plt.show()


   
def plot_lab_day3():
    file_path = get_file_path(3)
    data = load_file_data(file_path)
    optimal_travel, optimal_travel_rate, optimal_pitch, optimal_pitch_rate = data['x_star'][0], data['x_star'][1], data['x_star'][2], data['x_star'][3]
    travel, travel_rate, pitch, pitch_rate = data['travel'], data['travel_rate'], data['pitch'], data['pitch_rate']
    elevation, elevation_reference = data['elevation'], data['elevation_reference']
    time = data['time'].squeeze()
    print(time.shape)
    print(optimal_travel.shape)


    # Plotting
    fig, axs = plt.subplots(2, 1, figsize=(10, 20))  # Creating 2 subplots

    # Each subplot for each pair of variables
    axs[0].plot(time, travel.squeeze(), label='Actual Travel',linewidth=LineWidth)
    axs[0].plot(time, optimal_travel.squeeze(), label='Optimal Travel', linestyle='--',linewidth=LineWidth)
    axs[0].set_title('Travel vs Optimal Travel',fontsize=TitleSize)
    axs[0].set_xlabel('Time (s)',fontsize=FontSize)
    axs[0].set_ylabel('Travel (deg)',fontsize=FontSize)
    axs[0].legend(fontsize=FontSize)
    axs[0].grid(True)    
    axs[0].set_xlim(0,20) 
    axs[0].tick_params(axis='both', which='major', labelsize=FontSize)

    axs[1].plot(time, pitch.squeeze(), label='Actual Pitch',linewidth=LineWidth)
    axs[1].plot(time, optimal_pitch.squeeze(), label='Optimal Pitch', linestyle='--',linewidth=LineWidth)
    axs[1].set_title('Pitch vs Optimal Pitch',fontsize=TitleSize)
    axs[1].set_xlabel('Time (s)',fontsize=FontSize)
    axs[1].set_ylabel('Pitch (deg)',fontsize=FontSize)
    axs[1].legend(fontsize=FontSize)
    axs[1].grid(True)
    axs[1].set_xlim(0,20)
    axs[1].tick_params(axis='both', which='major', labelsize=FontSize)

    plt.subplots_adjust(hspace=0.4)
    font = {'family': 'serif', 'color': 'darkred', 'weight': 'normal', 'size': 12}

    

    plt.show()


def plot_lab_day4():
    file_path = get_file_path(4)
    data = load_file_data(file_path)


def main():
    while True:
        os.system('clear')
        print("Select Lab Day to plot:")
        print("1. Lab Day 2")
        print("2. Lab Day 3")
        print("3. Lab Day 4")
        print("4. Exit")
        choice = int(input("Enter your choice: "))
        if choice == 1:
            plot_lab_day2()
        elif choice == 2:
            plot_lab_day3()
        elif choice == 3:
            plot_lab_day4()
        elif choice == 4:
            break
        else:
            os.system('clear')
            print("Invalid choice.")

if __name__ == '__main__':
    main()
