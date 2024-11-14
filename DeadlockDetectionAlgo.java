import java.util.Scanner;
import java.util.Arrays;
import java.util.ArrayList;

public class DeadlockDetectionAlgo
{
    public static void main(String[] args)
    {
        ArrayList<Integer> safeSequence = new ArrayList<>();
        Scanner scanner = new Scanner(System.in);
        System.out.println("Enter total number of processes:");
        int totalNumberOfProcesses = scanner.nextInt();
        System.out.println("Enter total type of resources:");
        int totalTypesOfResources = scanner.nextInt();
        int[][] allocatedMatrix = new int[totalNumberOfProcesses][totalTypesOfResources+1];
        for(int i = 0 ; i<totalNumberOfProcesses;i++)
        {
            System.out.println("Enter process Id:");
            allocatedMatrix[i][0] = scanner.nextInt();
            for(int j = 1 ; j <= totalTypesOfResources ; j++)
            {
                System.out.println("For process with Id "+allocatedMatrix[i][0]+", enter allocated instance of R"+(j-1)+":");
                allocatedMatrix[i][j] = scanner.nextInt();
            }
        }



        int[][] requested = new int[totalNumberOfProcesses][totalTypesOfResources+1];
        for(int i = 0 ; i < totalNumberOfProcesses ; i++)
        {
            requested[i][0] = allocatedMatrix[i][0];
            for(int j = 1 ; j <= totalTypesOfResources ; j++)
            {
                System.out.println("For process with Id "+allocatedMatrix[i][0]+", enter requested instance of R"+j+":");
                requested[i][j] = scanner.nextInt();
            }
        }

        int[] totalResources =  new int[totalTypesOfResources];
        for(int i = 0 ; i< totalTypesOfResources;i++)
        {
            System.out.println("Enter total resources of R"+i+":");
            totalResources[i] = scanner.nextInt();
        }


        int[] allocated = new int[totalTypesOfResources];

        //Calculate allocated
        for(int[] row:allocatedMatrix)
        {
            for(int i = 1; i <= totalTypesOfResources;i++)
            {
                allocated[i-1] += row[i];
            }
        }

        int[] available = new int[totalTypesOfResources];
        for(int i = 0 ; i < totalTypesOfResources ;i++)
        {
            available[i] = totalResources[i] - allocated[i];
        }

        boolean[] finish = new boolean[totalNumberOfProcesses];

        System.out.println("Allocated Matrix:");
        for(int[] element:allocatedMatrix)
            System.out.println(Arrays.toString(element));

        System.out.println("Requested Matrix:");
        for(int[] element:requested)
            System.out.println(Arrays.toString(element));

        System.out.println("Total resources:"+Arrays.toString(totalResources));

        System.out.println("Allocated Resources:"+Arrays.toString(allocated));

        System.out.println("Available Resources:"+Arrays.toString(available));

        boolean flag;
        for(int i = 0 ; i < totalNumberOfProcesses ; i++)
        {
            flag = false;
            for(int j = 1 ; j <= totalTypesOfResources ;j++)
            {
                if(allocatedMatrix[i][j]!=0)
                {
                    flag = true;
                    break;
                }
            }
            if(flag)
                finish[i] = false;
            else
                finish[i] = true;
        }

        flag = true;
        boolean flag2 = true;
        while(flag)
        {
            flag = false;
            for(int i = 0 ; i < totalNumberOfProcesses ; i++)
            {
                if(!finish[i])
                {
                    for (int j = 1; j <= totalTypesOfResources; j++)
                    {
                        if (requested[i][j] <= available[j - 1])
                            flag2 = true;
                        else
                        {
                            flag2 = false;
                            break;
                        }
                    }
                    if (flag2)
                    {
                        flag = true;
                        for (int j = 0 ; j < totalTypesOfResources ; j++)
                            available[j] += allocatedMatrix[i][j+1];
                        finish[i] = true;
                        System.out.println("Completed execution of process with Id "+requested[i][0]+".Available Now:"+Arrays.toString(available));
                        safeSequence.add(requested[i][0]);
                    }
                }
            }
        }

        for(boolean element:finish)
        {
            if(!element)
            {
                System.out.println("The system is in deadlock.Processes that are in deadlock are:-");
                for(int[] row:requested)
                {
                    if(!safeSequence.contains(row[0]))
                        System.out.println(row[0]);
                }
                System.out.println("Finish:"+Arrays.toString(finish));
                System.exit(1);
            }
        }

        System.out.println("The system is in safe state.Safe sequence:"+safeSequence);

    }
}
