import java.util.ArrayList;
import  java.util.Arrays;
import  java.util.Scanner;

public class Bankers
{
    private static void printMatrix(int[][] printThis)
    {
        for(int[] row:printThis)
            System.out.println(Arrays.toString(row));
    }

    public static void main(String[] args)
    {
        Scanner scanner = new Scanner(System.in);
        System.out.println("\nEnter total Number of processes:");

        int processes = scanner.nextInt();

        System.out.println("\nEnter total Number of resources");
        int numberOfResources = scanner .nextInt();

        int[][] allocated = new int[processes][numberOfResources+1];
        int[][] max = new int[processes][numberOfResources+1];
        int[][] need = new int[processes][numberOfResources+1];

        for(int i=0; i <processes;i++)
        {
            System.out.println("\nEnter process Id:");
            int processId = scanner.nextInt();
            allocated[i][0] = processId;
            max[i][0] = processId;
            need[i][0] =processId;
            for(int j = 1; j<=numberOfResources;j++)
            {
                System.out.println("\nEnter allocated resource for R"+(j-1)+":");
                allocated[i][j]=scanner.nextInt();
            }
        }

        for(int i = 0 ; i<processes;i++)
        {
            for(int j =1 ; j <= numberOfResources;j++)
            {
                System.out.println("\nFor process with processId "+max[i][0] + " max instances of resource R"+(j-1)+":");
                max[i][j] = scanner.nextInt();
            }
        }

        int[] totalResources = new int[numberOfResources];
        for(int i = 0 ; i < numberOfResources;i++)
        {
            System.out.println("Enter total instances of R"+i+":");
            totalResources[i] = scanner.nextInt();
        }

        for(int i = 0 ; i<processes;i++)
        {
            for(int j =1 ; j <= numberOfResources;j++)
            {
                need[i][j] = max[i][j] - allocated[i][j];
            }
        }


        System.out.println("Allocated Matrix:\n");
        Bankers.printMatrix(allocated);

        System.out.println("Max matrix:\n");
        Bankers.printMatrix(max);

        System.out.println("Need matrix:\n");
        Bankers.printMatrix(need);

        //Calculate Available
        int[] allocatedResources = new int[numberOfResources];
        int sum = 0;
        for(int i = 1 ; i<= numberOfResources;i++)
        {
            for(int j = 0 ; j < processes;j++)
            {
                sum += allocated[j][i];
            }
            allocatedResources[i-1]=sum;
            sum = 0;
        }

        int[] available = new int[numberOfResources];
        for(int i = 0 ; i < numberOfResources;i++)
            available[i] = totalResources[i] - allocatedResources[i];

        boolean[] statusOfProcess = new boolean[processes];

        System.out.println("\nAllocated:"+Arrays.toString(allocatedResources));
        System.out.println("\nAvailable:"+Arrays.toString(available));

        ArrayList<Integer>safeSequence = new ArrayList<>();
        boolean flag1 = true;
        while(flag1)
        {
            flag1 = false;
            boolean flag2 = false;
            int indexForAvailable = 0;

            for(int i = 0 ; i < processes;i++)
            {
                if(!statusOfProcess[i])
                {
                    flag2 = false;
                    for(int j = 1;j<=numberOfResources;j++)
                    {
                        if(need[i][j] > available[j-1])
                        {
                            flag2 = true;
                            break;
                        }
                    }

                    if(!flag2)
                    {
                        flag1 = true;
                        statusOfProcess[i] = true;
                        safeSequence.add(allocated[i][0]);
                        for(int j = 1;j<=numberOfResources ;j++)
                            available[j-1] += allocated[i][j];

                        System.out.println("Available resource after execution of process Id "+allocated[i][0]+":"+Arrays.toString(available));
                    }
                }
            }
        }

        for(boolean element:statusOfProcess)
        {
            if(!element){
                System.out.println("The system is in unsafe state");
                System.exit(0);
            }
        }

        System.out.println("Safe sequence:"+safeSequence);

    }
}