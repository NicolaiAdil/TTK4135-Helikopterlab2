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

    #day2,3 has 15 elements (and one zero row due to me being noob), day4 has 18 elements
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

    # Load data for three different q-values
    data_q_equals_0_point_12 = load_file_data("data/LabDay2_q_equals_0_point_12.mat")
    data_q_equals_1_point_12 = load_file_data("data/LabDay2_q_equals_1_point_2.mat")
    data_q_equals_12 = load_file_data("data/LabDay2_q_equals_12.mat")

    # Extract time and pitch reference data for each q-value dataset
    time_0_12 = data_q_equals_0_point_12['time'].squeeze()
    time_1_12 = data_q_equals_1_point_12['time'].squeeze()
    time_12 = data_q_equals_12['time'].squeeze()

    # Limit data to the first 20 seconds
    mask_0_12 = time_0_12 <= 20
    mask_1_12 = time_1_12 <= 20
    mask_12 = time_12 <= 20

    if choice == 1:
        pitch_0_12 = data_q_equals_0_point_12['pitch'].squeeze()
        pitch_1_12 = data_q_equals_1_point_12['pitch'].squeeze()
        pitch_12 = data_q_equals_12['pitch'].squeeze()

        travel_0_12 = data_q_equals_0_point_12['travel'].squeeze()
        travel_1_12 = data_q_equals_1_point_12['travel'].squeeze()
        travel_12 = data_q_equals_12['travel'].squeeze()
        # Plotting for choice 1: Travel and Pitch for different q-values
        fig, axs = plt.subplots(2, 1, figsize=(12, 12))
        
        # Plot for Travel
        axs[0].plot(time_0_12, travel_0_12, 'r', label='q = 0.12', linewidth=LineWidth)
        axs[0].plot(time_1_12, travel_1_12, 'g', label='q = 1.2', linewidth=LineWidth)
        axs[0].plot(time_12, travel_12, 'b', label='q = 12', linewidth=LineWidth)
        axs[0].set_title('Travel for Different q-values', fontsize=TitleSize)
        axs[0].set_xlabel('Time (s)', fontsize=FontSize)
        axs[0].set_ylabel('Travel (deg)', fontsize=FontSize)
        axs[0].legend(fontsize=FontSize)
        axs[0].grid(True)
        axs[0].tick_params(axis='both', which='major', labelsize=FontSize)

        # Plot for Pitch
        axs[1].plot(time_0_12, pitch_0_12, 'r', label='q = 0.12', linewidth=LineWidth)
        axs[1].plot(time_1_12, pitch_1_12, 'g', label='q = 1.2', linewidth=LineWidth)
        axs[1].plot(time_12, pitch_12, 'b', label='q = 12', linewidth=LineWidth)
        axs[1].set_title('Pitch for Different q-values', fontsize=TitleSize)
        axs[1].set_xlabel('Time (s)', fontsize=FontSize)
        axs[1].set_ylabel('Pitch (deg)', fontsize=FontSize)
        axs[1].legend(fontsize=FontSize)
        axs[1].grid(True)
        axs[1].tick_params(axis='both', which='major', labelsize=FontSize)

        plt.tight_layout()
        plt.show()
        
    elif choice == 2:
        pitch_ref_0_12 = data_q_equals_0_point_12['pitch_reference'].squeeze()
        pitch_ref_1_12 = data_q_equals_1_point_12['pitch_reference'].squeeze()
        pitch_ref_12 = data_q_equals_12['pitch_reference'].squeeze()

    
        # Plotting
        fig, ax = plt.subplots(figsize=(12, 6))  # Creating a subplot for pitch comparison

        # Plotting pitch data with dotted lines
        ax.plot(time_0_12[mask_0_12], pitch_ref_0_12[mask_0_12], 'r--', label='q = 0.12', linewidth=LineWidth)
        ax.plot(time_1_12[mask_1_12], pitch_ref_1_12[mask_1_12], 'g--', label='q = 1.2', linewidth=LineWidth)
        ax.plot(time_12[mask_12], pitch_ref_12[mask_12], 'b--', label='q = 12', linewidth=LineWidth)

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
    optimal_travel, optimal_pitch, = data['x_star'][0], data['x_star'][2]
    travel, pitch = data['travel'],data['pitch']
    time = data['time'].squeeze()
    print(time.shape)
    print(optimal_travel.shape)


    # Plotting
    _, axs = plt.subplots(2, 1, figsize=(10, 20))  # Creating 2 subplots

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
    plt.show()


def plot_lab_day4():
    file_path = get_file_path(4)
    data = load_file_data(file_path)
    print("Keys:", data.keys())
    print(data['x_star'])

    travel, pitch, elevation = data['travel'],  data['pitch'], data['elevation']
    optimal_travel, optimal_pitch, optimal_elevation = data['x_star'][0], data['x_star'][2], data['x_star'][4]
    time = data['time'].squeeze()

    _, axs = plt.subplots(3, 1, figsize=(10, 20))  # Creating 2 subplots

    # Each subplot for each pair of variables
    axs[0].plot(time, travel.squeeze(), label='Actual Travel',linewidth=LineWidth)
    axs[0].plot(time, optimal_travel.squeeze(), label='Optimal Travel', linestyle='--',linewidth=LineWidth)
    axs[0].set_title('Travel vs Optimal Travel',fontsize=TitleSize)
    axs[0].set_xlabel('Time (s)',fontsize=FontSize)
    axs[0].set_ylabel('Travel (deg)',fontsize=FontSize)
    axs[0].legend(fontsize=FontSize)
    axs[0].grid(True)    
    axs[0].set_xlim(0,25) 
    axs[0].tick_params(axis='both', which='major', labelsize=FontSize)

    axs[1].plot(time, pitch.squeeze(), label='Actual Pitch',linewidth=LineWidth)
    axs[1].plot(time, optimal_pitch.squeeze(), label='Optimal Pitch', linestyle='--',linewidth=LineWidth)
    axs[1].set_title('Pitch vs Optimal Pitch',fontsize=TitleSize)
    axs[1].set_xlabel('Time (s)',fontsize=FontSize)
    axs[1].set_ylabel('Pitch (deg)',fontsize=FontSize)
    axs[1].legend(fontsize=FontSize)
    axs[1].grid(True)
    axs[1].set_xlim(0,25)
    axs[1].tick_params(axis='both', which='major', labelsize=FontSize)

    axs[2].plot(time, elevation.squeeze(), label='Actual Elevation',linewidth=LineWidth)
    axs[2].plot(time, optimal_elevation.squeeze(), label='Optimal Elevation', linestyle='--',linewidth=LineWidth)
    axs[2].set_title('Elevation vs Optimal Eleavtion',fontsize=TitleSize)
    axs[2].set_xlabel('Time (s)',fontsize=FontSize)
    axs[2].set_ylabel('Elevation (deg)',fontsize=FontSize)
    axs[2].legend(fontsize=FontSize)
    axs[2].grid(True)
    axs[2].set_xlim(0,25)
    axs[2].set_ylim(-10,15)
    axs[2].tick_params(axis='both', which='major', labelsize=FontSize)

    plt.subplots_adjust(hspace=0.5)
    plt.show()


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
            break
        if choice == 2:
            plot_lab_day2()
        elif choice == 3:
            plot_lab_day3()
        elif choice == 4:
            plot_lab_day4()
        else:
            print("Invalid choice.")

if __name__ == '__main__':
    main()
