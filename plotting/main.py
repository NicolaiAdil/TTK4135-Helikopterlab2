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

    if choice == 1:

        # Limit data to the first 20 seconds
        mask_0_12 = time_0_12 <= 35
        mask_1_12 = time_1_12 <= 35
        mask_12 = time_12 <= 35

        pitch_0_12 = data_q_equals_0_point_12['pitch'].squeeze()[mask_0_12]
        pitch_1_12 = data_q_equals_1_point_12['pitch'].squeeze()[mask_1_12]
        pitch_12 = data_q_equals_12['pitch'].squeeze()[mask_12]

        travel_0_12 = data_q_equals_0_point_12['travel'].squeeze()[mask_0_12]
        travel_1_12 = data_q_equals_1_point_12['travel'].squeeze()[mask_1_12]
        travel_12 = data_q_equals_12['travel'].squeeze()[mask_12]

        # Plotting for choice 1: Travel and Pitch for different q-values
        fig, axs = plt.subplots(2, 1, figsize=(12, 12))
        
        # Plot for Travel
        axs[0].plot(time_0_12[mask_0_12], travel_0_12, 'r', label='q = 0.12', linewidth=LineWidth)
        axs[0].plot(time_1_12[mask_1_12], travel_1_12, 'g', label='q = 1.2', linewidth=LineWidth)
        axs[0].plot(time_12[mask_12], travel_12, 'b', label='q = 12', linewidth=LineWidth)
        axs[0].set_title('Travel for Different q-values', fontsize=TitleSize)
        axs[0].set_xlabel('Time (s)', fontsize=FontSize)
        axs[0].set_ylabel('Travel (deg)', fontsize=FontSize)
        axs[0].legend(fontsize=FontSize)
        axs[0].grid(True)
        axs[0].tick_params(axis='both', which='major', labelsize=FontSize)

        # Plot for Pitch
        axs[1].plot(time_0_12[mask_0_12], pitch_0_12, 'r', label='q = 0.12', linewidth=LineWidth)
        axs[1].plot(time_1_12[mask_1_12], pitch_1_12, 'g', label='q = 1.2', linewidth=LineWidth)
        axs[1].plot(time_12[mask_12], pitch_12, 'b', label='q = 12', linewidth=LineWidth)
        axs[1].set_title('Pitch for Different q-values', fontsize=TitleSize)
        axs[1].set_xlabel('Time (s)', fontsize=FontSize)
        axs[1].set_ylabel('Pitch (deg)', fontsize=FontSize)
        axs[1].legend(fontsize=FontSize)
        axs[1].grid(True)
        axs[1].tick_params(axis='both', which='major', labelsize=FontSize)

        plt.tight_layout()
        plt.show()
        
    elif choice == 2:
        mask_0_12 = time_0_12 <= 20
        mask_1_12 = time_1_12 <= 20
        mask_12 = time_12 <= 20

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
    print("Select Plotting Option:")
    print("1. Plot Travel, Pitch, and Elevation vs Optimal")
    print("2. Plot Different N")
    print("3. Plot Different q")
    print("4. Exit")
    choice = int(input("Enter your choice: "))
    
    file_path = get_file_path(4)
    data = load_file_data(file_path)
    
    travel, pitch, elevation = data['travel'], data['pitch'], data['elevation']
    optimal_travel, optimal_pitch, optimal_elevation = data['x_star'][0], data['x_star'][2], data['x_star'][4]
    time = data['time'].squeeze()

    if choice == 1:
        _, axs = plt.subplots(3, 1, figsize=(10, 20))  # Creating 2 subplots

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

    elif choice == 2:

        data60 = load_file_data("data/LabDay4_N_equals_60_q_equals_1_unit_tune")
        data40 = load_file_data("data/LabDay4_N_equals_40_q_equals_1_unit_tune")

        travel60, elevation60 = data60['travel'], data60['elevation']
        optimal_travel60,  optimal_elevation60 = data60['x_star'][0], data60['x_star'][4]

        time60 = data60['time'].squeeze()

        travel40, elevation40 = data40['travel'], data40['elevation']
        optimal_travel40,  optimal_elevation40 = data40['x_star'][0], data40['x_star'][4]

        time40 = data40['time'].squeeze()

        _, axs = plt.subplots(2, 1, figsize=(10, 20))  # Creating 2 subplots

        axs[0].plot(time60, travel60.squeeze(), label='Actual Travel N = 60',linewidth=LineWidth,color='blue')
        axs[0].plot(time60, optimal_travel60.squeeze(), label='Optimal Travel N = 60', linestyle='--',linewidth=LineWidth,color='blue')
        axs[0].plot(time40, travel40.squeeze(), label='Actual Travel N = 40',linewidth=LineWidth,color='orange')
        axs[0].plot(time40, optimal_travel40.squeeze(), label='Optimal Travel N = 40', linestyle='--',linewidth=LineWidth,color='orange')
        axs[0].set_title('Travel vs Optimal Travel',fontsize=TitleSize)
        axs[0].set_xlabel('Time (s)',fontsize=FontSize)
        axs[0].set_ylabel('Travel (deg)',fontsize=FontSize)
        axs[0].legend(fontsize=FontSize)
        axs[0].grid(True)    
        axs[0].set_xlim(0,25) 
        axs[0].tick_params(axis='both', which='major', labelsize=FontSize)

        axs[1].plot(time60, elevation60.squeeze(), label='Actual Elevation N = 60',linewidth=LineWidth,color='blue')
        axs[1].plot(time60, optimal_elevation60.squeeze(), label='Optimal Elevation = 60', linestyle='--',linewidth=LineWidth,color='blue')
        axs[1].plot(time40, elevation40.squeeze(), label='Actual Elevation N = 40',linewidth=LineWidth,color='orange')
        axs[1].plot(time40, optimal_elevation40.squeeze(), label='Optimal Elevation N = 40', linestyle='--',linewidth=LineWidth,color='orange')
        axs[1].set_title('Pitch vs Optimal Pitch',fontsize=TitleSize)
        axs[1].set_xlabel('Time (s)',fontsize=FontSize)
        axs[1].set_ylabel('Pitch (deg)',fontsize=FontSize)
        axs[1].legend(fontsize=FontSize)
        axs[1].grid(True)
        axs[1].set_xlim(0,25)
        axs[1].tick_params(axis='both', which='major', labelsize=FontSize)

        plt.subplots_adjust(hspace=0.5)
        plt.show()

    elif choice == 3:
        _, axs = plt.subplots(1, 1, figsize=(10, 10))  # Creating 2 subplots

        axs.plot(time, travel.squeeze(), label='Actual Travel',linewidth=LineWidth)
        axs.plot(time, optimal_travel.squeeze(), label='Optimal Travel', linestyle='--',linewidth=LineWidth)
        axs.set_title('Travel vs Optimal Travel',fontsize=TitleSize)
        axs.set_xlabel('Time (s)',fontsize=FontSize)
        axs.set_ylabel('Travel (deg)',fontsize=FontSize)
        axs.legend(fontsize=FontSize)
        axs.grid(True)    
        axs.set_xlim(0,25) 
        axs.tick_params(axis='both', which='major', labelsize=FontSize)

        plt.show()

    elif choice == 4:
        pass  # Exit the function

    else:
        print("Invalid choice. Exiting.")



def main():
    while True:
        os.system('clear')
        print("Select Lab Day to plot:")
        print("1. Exit")
        print("2. Lab Day 2")
        print("3. Lab Day 3")
        print("4. Lab Day 4")
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
