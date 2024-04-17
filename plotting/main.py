import scipy.io
import matplotlib.pyplot as plt
import os

def load_mat_file(file_path):
    # Load the MATLAB file
    mat_data = scipy.io.loadmat(file_path)
    return mat_data


def plot_lab_day2():
    files = [f for f in os.listdir('data') if f.startswith('LabDay2')]
    for i, file in enumerate(files):
        print(f"{i+1}. {file}")
    choice = int(input("Choose a file to plot: ")) - 1

    file_path = os.path.join('data', files[choice])
    data = load_mat_file(file_path)
    print(data.keys())
    print(data['x_star'].shape)
    print(data['x_star'])
    optimal_travel, optimal_travel_rate, optimal_pitch, optimal_pitch_rate = data['x_star'][0], data['x_star'][1], data['x_star'][2], data['x_star'][3]
    travel, travel_rate, pitch, pitch_rate = data['travel'], data['travel_rate'], data['pitch'], data['pitch_rate']
    time = data['time']

    # Plotting
    fig, axs = plt.subplots(4, 1, figsize=(10, 20))  # Creating 4 subplots

    # Each subplot for each pair of variables
    axs[0].plot(time, travel.squeeze(), label='Actual Travel')
    axs[0].plot(time, optimal_travel.squeeze(), label='Optimal Travel', linestyle='--')
    axs[0].set_title('Travel vs Optimal Travel')
    axs[0].set_xlabel('Time (s)')
    axs[0].set_ylabel('Travel')
    axs[0].legend()
    axs[0].grid(True)

    axs[1].plot(time, travel_rate.squeeze(), label='Actual Travel Rate')
    axs[1].plot(time, optimal_travel_rate.squeeze(), label='Optimal Travel Rate', linestyle='--')
    axs[1].set_title('Travel Rate vs Optimal Travel Rate')
    axs[1].set_xlabel('Time (s)')
    axs[1].set_ylabel('Travel Rate')
    axs[1].legend()
    axs[1].grid(True)

    axs[2].plot(time, pitch.squeeze(), label='Actual Pitch')
    axs[2].plot(time, optimal_pitch.squeeze(), label='Optimal Pitch', linestyle='--')
    axs[2].set_title('Pitch vs Optimal Pitch')
    axs[2].set_xlabel('Time (s)')
    axs[2].set_ylabel('Pitch')
    axs[2].legend()
    axs[2].grid(True)

    axs[3].plot(time, pitch_rate.squeeze(), label='Actual Pitch Rate')
    axs[3].plot(time, optimal_pitch_rate.squeeze(), label='Optimal Pitch Rate', linestyle='--')
    axs[3].set_title('Pitch Rate vs Optimal Pitch Rate')
    axs[3].set_xlabel('Time (s)')
    axs[3].set_ylabel('Pitch Rate')
    axs[3].legend()
    axs[3].grid(True)

    plt.tight_layout()
    plt.show()



   
def plot_lab_day3():
    files = [f for f in os.listdir('data') if f.startswith('LabDay3')]
    for i, file in enumerate(files):
        print(f"{i+1}. {file}")
    choice = int(input("Choose a file to plot: ")) - 1
    file_path = os.path.join('data', files[choice])
    data = load_mat_file(file_path)

def plot_lab_day4():
    files = [f for f in os.listdir('data') if f.startswith('LabDay4')]
    for i, file in enumerate(files):
        print(f"{i+1}. {file}")
    choice = int(input("Choose a file to plot: ")) - 1
    file_path = os.path.join('data', files[choice])
    data = load_mat_file(file_path)

def main():
    while True:
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
