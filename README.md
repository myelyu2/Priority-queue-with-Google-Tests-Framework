# Priority queue

<h3>Project background information</h3>
This priority queue class is used for an imaginary patient queue at a hopsital. As each new patient checks into the hospital, the staff assesses the patietn’s injuries and gives them an integer priority rating. Smaller integers represent greater urgency, for example a patient of priority 1 is more urgent should receive care before a patient of priority 2. Once a doctor is ready to see a patient, the patient with the most urgent priority (i.e. with the lowest number rating) is seen first. However, this class has more general purpose abstraction class. Therefore, it works more generally for many types of priority queues.
</br>
The basic design of a priority queue is that, regardless of the order in which you add/enqueue the elements, when you remove/dequeue them, the one with the lowest priority number comes out first, then second-lowest, and so one, with the highest-numbered (i.e. least urgent) item coming out last. In this implementation of the priority queue, if two or more elements in the queue have the same priority, it break ties by choosing the element who was placed in the queue first. This means that if an element in placed in the queue at priority K and there are already other elements in the queue with priority K, this new element should be placed after them. For example, imagine you have a patient queue for a hospital and patients arrive in this order:</br>
• "Dolores" with priority 5</br>
• "Bernard" with priority 4</br>
• "Arnold" with priority 8</br>
• "William" with priority 5</br>
• "Teddy" with priority 5</br>
• "Ford" with priority 2</br>
If you were to dequeue the patients to process them, they would come out in this order: Ford, Bernard, Dolores, William, Teddy, Arnold.</br>
</br>
A priority queue can internally store its elements in sorted or unsorted order; all the matters is that when the elements are dequeued, they come out in sorted order by priority.</br>


<h3>Implementations Details</h3>
“Under the hood” of the priority queue abstraction could be a C array, a linked list, a vector, and many other options. We are going to implement with a custom binary search tree. Later this semester, we are going to learn about a heap data structure (which, by the way, has nothing to do with heap portion of memory). A priority queue is best and most naturally implemented with a heap. But we like to do things that are more painful. So, instead, we will implement with a <b>custom binary search tree</b>. For example, if add these elements to the queue:</br>
• "Dolores" with priority 5</br>
• "Bernard" with priority 4</br>
• "Arnold" with priority 8</br>
• "Ford" with priority 2</br>
• “Jasmine” with priority 6</br>
• “Carlos” with priority 11</br>
The tree will look like this. Notice that the BST is is being created based on the priority number not the value.</br>
<img width="539" alt="Screen Shot 2022-07-15 at 5 04 46 PM" src="https://user-images.githubusercontent.com/89800281/179317177-70620379-7bf9-405b-8113-aa6dbac96f5c.png"></br>

You might be wondering, how do we use a BST if we have priority ties in priority? We know by now that BST’s require unique values in order to stay in sorted order. Therefore, let’s say we add these elements to the queue:</br>
• "Dolores" with priority 5</br>
• "Bernard" with priority 4</br>
• "Arnold" with priority 8</br>
• "Ford" with priority 2</br>
• “Jasmine” with priority 6</br>
• “Carlos” with priority 11</br>
• "William" with priority 8</br>
• "Teddy" with priority 8</br>
We are going to implement our priority queue by adding a third dimension of linkages. Ahh! This is a visual of what it will look like:</br>
<img width="585" alt="Screen Shot 2022-07-15 at 5 05 34 PM" src="https://user-images.githubusercontent.com/89800281/179317260-9dd46234-f72e-4754-9bd8-2e54e41ba994.png">

The data structure you are going to use a binary search tree. The node structure looks like this:
<img width="707" alt="Screen Shot 2022-07-15 at 5 05 54 PM" src="https://user-images.githubusercontent.com/89800281/179317293-a8762608-a2c1-45b5-b89a-17649c8b864f.png">

<h3>Testing</h3>
Developed scalable tests, using the Google Tests Framework.

