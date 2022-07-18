# Complexity and recursion: Distribution and spatial overlap

## **1. About this project**

This problem is a **covering problem**.   
We will therefore use **recursivity** to solve it. Also, the objective is to **optimize the code** by finding the number of exact solutions as fast as possible and thus **reducing the complexity**.

We have a **schedule** that looks like this: 

<div id="schedule" align = "center">
  <img src="https://user-images.githubusercontent.com/105392989/176171755-5e3a58f6-65ef-420b-b557-4e2ecc97125e.png" width="500">
</div>

For a given date, the following **10 items must be filed**:

<div id="10 items" align = "center">
  <img src="https://user-images.githubusercontent.com/105392989/176171872-8b11f234-48f9-4cc5-844b-8aa5ca608052.png" width="750">
</div>

However, the only 3 uncovered boxes correspond to the date indicated *(number, day, month)*.   
Here are some additional **constraints**:
  - The blue pieces must not overlap
  - The sum of the unit squares of the green area is equal to the sum of the squares that constitute the blue pieces plus 3 units.
  - It is possible to rotate each part by 90, 180 or 270°

The question is whether this is possible for all days of the year.  
**Result :** It is not possible, there are combinations that make it impossible to overlap with the ten pieces, no matter how they are rotated and organized.

The solutions we find are indicated in the file **CR_Distribution**. We find there :
  - The month
  - The date number
  - The day
  - The number of solution
  - The execution time to find the solution

<div id="results" align = "center">
  <img src="https://user-images.githubusercontent.com/105392989/176172106-372f3892-4ab8-45f4-93f0-ed7c8df5ffe6.png" width="750">
</div>

This project was done in a school context with two other classmates.
