#include<stdio.h>
#include<ctype.h>
#include<math.h>
#include<string.h>
#include<GL/glut.h>
float offset = 0.0;
bool box_clicked = false;
int temp_x, temp_y, temp_item;
bool prev_search = false;
enum color_code {RED, GREEN, BLUE};
struct node {
	int data;
	struct node *left;
	struct node *right;
};
typedef struct node* NODE;
NODE root = NULL;
char query_string[] ="Enter an Integer to be inserted and Hit Enter : ";
char query_string2[]="Enter an Integer to be searched and Hit Enter :";
char query_string3[]="Enter an Integer to be deleted and Hit Enter :";
char query_string4[]="The preorder is:";
char query_string5[]="The inorder is:";
char query_string6[]="The postorder is:";
char not_found[] = "Search failed!!Item Not found";
char not_found2[]= "Item cannot be deleted!! Item not Found";
float node_color[3] = {0.419608 , 0.556863 , 0.137255};
void display();
int count_digit(int n){
	return (n==0)?0:floor(log10(n)+1);
}
void to_string(char *s, int num){
	int r, d;
	d = count_digit(abs(num));
	if(num==0)
		s[0]='0';
	if(num<0){
		s[0] = '-';
		d++;
	}
	s[d]='\0';
	num = abs(num);
	while(num!=0){
		r = num%10;
		s[--d] = r+'0';
		num/=10;
	}
}
void display_string(char s[], float x, float y, float z = 0.0){
	glRasterPos3f(x, y, z);
	for(int i=0; s[i]; i++){

		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
	}
	glFlush();

}
void clear_input_region(){
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex2f(100, 630);
		glVertex2f(100, 700);
		glVertex2f(700, 650);
		glVertex2f(680, 700);
	glEnd();
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex2f(40, 40);
		glVertex2f(40, 100);
		glVertex2f(650, 100);
		glVertex2f(650, 40);
	glEnd();
	glFlush();
}
static int i=0;
class binary_search_tree {
	private:
		NODE root;
		int root_centre_x, root_centre_y;
		int node_width;
		float line_color[3];
		char data_string[10];
		NODE get_minimum(NODE temp, NODE *par){
			if(temp->left == NULL)
				return temp;
			*par = temp;
			return get_minimum(temp->left, par);
		}
	public:
		binary_search_tree(){
			root = NULL;
			root_centre_x = 600;
			root_centre_y = 600;
			node_width = 30;
			line_color[RED] = 0.36;
			line_color[GREEN] = 0.25;
			line_color[BLUE] = 0.20;
 
		}
		void draw_node(int item, int centre_x, int centre_y, bool write_value = true){
			char data_string[10];
			glBegin(GL_POLYGON);
				glVertex2f(centre_x + node_width, centre_y + node_width);
				glVertex2f(centre_x - node_width, centre_y + node_width);
				glVertex2f(centre_x - node_width, centre_y - node_width);
				glVertex2f(centre_x + node_width, centre_y - node_width);
			glEnd();
			if(write_value){
				to_string(data_string, item);
				glColor3f(1.0, 1.0, 0.0);
				display_string(data_string, centre_x - node_width + 10, centre_y);
			}
		}
		void draw_arrow(int par_x, int par_y, int node_x, int node_y, bool color = true){
			if(color)
				glColor3f(line_color[RED], line_color[GREEN], line_color[BLUE]);
		
			glBegin(GL_LINES);
				glVertex2f(par_x, par_y - node_width);
				glVertex2f(node_x, node_y + node_width);
			glEnd();
		}
		NODE insert(int item){
			int num_of_nodes = 1;
			int node_x = root_centre_x, node_y = root_centre_y;
			int par_x, par_y;
			NODE temp, par = NULL, new_node;
			if(root == NULL){
				root = (NODE)malloc(sizeof(struct node));
				if(root == NULL){
					printf("There was an error in creating the node\n");
					return NULL;
				}
				root->data = item;
				root->left = root->right = NULL;
				glColor3f(node_color[RED], node_color[GREEN], node_color[BLUE]);
				draw_node(item, node_x, node_y);
				return root;
			}	
			temp = root;
			while(temp != NULL){
				num_of_nodes *= 2;
				par_x = node_x;
				par_y = node_y;
				node_y -= 100;
				par = temp;
				if(item < temp->data) {
					temp = temp->left;	
					node_x -= root_centre_x/num_of_nodes;			
				}
				else {
					temp = temp->right;
					node_x += root_centre_x/num_of_nodes;
				}		
			}
			new_node = (NODE)malloc(sizeof(struct node));
			if(new_node == NULL){
					printf("There was an error in creating the node\n");
					return NULL;
				}
			new_node->data = item;
			new_node->left = new_node->right = NULL;
			if(item < par->data)
				par->left = new_node;			
			else
				par->right = new_node;
			glColor3f(node_color[RED], node_color[GREEN], node_color[BLUE]);
			draw_node(item, node_x, node_y);
			draw_arrow(par_x, par_y, node_x, node_y);
			glFlush();
			return root;
		}
		NODE search(int item){
			int num_of_nodes = 1;
			int node_x = root_centre_x, node_y = root_centre_y;
			int par_x, par_y;
			NODE temp, par = NULL, new_node;
			if(root == NULL){
				glColor3f(1.0, 1.0, 0.0);
				display_string(not_found, 100, 650);
				return root;
			}	
			temp = root;
			while(temp != NULL){
				num_of_nodes *= 2;
				par_x = node_x;
				par_y = node_y;
				par = temp;
				if(item == temp->data){
					prev_search = true;
					temp_x = node_x;
					temp_y = node_y;
					temp_item = item;
					glColor3f(1.0, 0.0, 0.0);
					draw_node(item, node_x, node_y);
					glFlush();
					return temp;
				}
				else if(item < temp->data) {
					temp = temp->left;	
					node_y -= 100;
					node_x -= root_centre_x/num_of_nodes;			
				}
				else {
					temp = temp->right;
					node_y -= 100;
					node_x += root_centre_x/num_of_nodes;
				}
			}
			glColor3f(1.0, 1.0, 0.0);
			display_string(not_found, 100, 650);
			glFlush();
			return temp;
		}
		void pre_order(NODE temp, binary_search_tree temp_tree){
			if(temp != NULL){
				temp_tree.insert(temp->data);
				pre_order(temp->left, temp_tree);
				pre_order(temp->right, temp_tree);
			}
		}
		void delay()
		{
    			for(int i=0;i<22000;i++)
      			for(int j=0;j<22000;j++);
		}
		void preorder(NODE temp, binary_search_tree temp_tree){
			if(temp != NULL){
				temp_tree.insert(temp->data);
				delay();
				to_string(data_string, temp->data);
				glColor3f(1.0, 1.0, 0.0);
				i=i+1;
				display_string(data_string,300+(i*20), 50);
				search(temp->data);				
				delay();
				i=i+1;
				preorder(temp->left, temp_tree);
				preorder(temp->right, temp_tree);
				}
		}
		void inorder(NODE temp, binary_search_tree temp_tree){
			if(temp != NULL){
				temp_tree.insert(temp->data);
				delay();
				inorder(temp->left, temp_tree);
				to_string(data_string, temp->data);
				glColor3f(1.0, 1.0, 0.0);
				i=i+1;
				display_string(data_string,300+(i*20), 50);
				search(temp->data);
				delay();
				i=i+1;
				inorder(temp->right, temp_tree);
				}
			}
		void postorder(NODE temp, binary_search_tree temp_tree){
		if(temp != NULL){
				temp_tree.insert(temp->data);
				postorder(temp->left, temp_tree);
				delay();
				postorder(temp->right, temp_tree);
				to_string(data_string, temp->data);
				glColor3f(1.0, 1.0, 0.0);
				i=i+1;
				display_string(data_string,300+(i*20), 50);
				search(temp->data);
				delay();
				i=i+1;
			}
		}
		void draw_tree(){
			binary_search_tree temp_tree;
			display();	
			pre_order(root, temp_tree);
		}
		void preordertraversal(){
			binary_search_tree temp_tree;
			display();
			display_string(query_string4,100, 50);
			preorder(root, temp_tree);
			pre_order(root, temp_tree);
		}
		void inordertraversal(){
			binary_search_tree temp_tree;
			display();
			display_string(query_string5,100, 50);
			inorder(root, temp_tree);
			pre_order(root, temp_tree);
		}
		void postordertraversal(){
			binary_search_tree temp_tree;
			display();	
			display_string(query_string6,100, 50);
			postorder(root, temp_tree);
			pre_order(root, temp_tree);
		}
		NODE remove(int item){
			bool found = false;
			int num_of_nodes = 1;
			int node_x = root_centre_x, node_y = root_centre_y;
			int par_x, par_y;
			NODE temp = root, par = root, new_node;
			if(root == NULL){
				glColor3f(1.0, 1.0, 0.0);
				display_string(not_found2, 100, 650);
				return root;
			}
			while(temp != NULL){
				if(item == temp->data){
					found = true;
					if(temp == root){
						if(temp->left == NULL && temp->right == NULL){
							root = NULL;
							free(temp);
						}
						if(temp->left != NULL && temp->right != NULL){
							par = temp;
							NODE min_node = get_minimum(temp->right, &par);
							temp->data = min_node->data;	
							temp = min_node;
							item = min_node->data;
							continue;						
						}
						if(temp->left != NULL){
							root = temp->left;
							free(temp);
						}
						if(temp->right != NULL){
							root = temp->right;
							free(temp);
						}
					}
					else if(temp->left == NULL && temp->right == NULL){
						if(par->left == temp)
							par->left = NULL;
						else
							par->right = NULL;
						free(temp);
					}
					else if(temp->left != NULL && temp->right != NULL){
						par = temp;
						NODE min_node = get_minimum(temp->right, &par);
						temp->data = min_node->data;	
						temp = min_node;
						item = min_node->data;
						continue;
					}
					else {
						if(temp->left != NULL)
							(par->left == temp) ? (par->left = temp->left) : (par->right = temp->left);	
						else 
							(par->left == temp) ? (par->left = temp->right) : (par->right = temp->right);	
						free(temp);
					}	
					break;	
				}
				par = temp;
				if(item < temp->data)	
					temp = temp->left;
				else
					temp = temp->right;
			}
			if(!found){
				glColor3f(1.0, 1.0, 0.0);
				display_string(not_found2, 100, 650);
			}
			else
				draw_tree();
		}
};
binary_search_tree tree;
NODE (binary_search_tree::*operation)(int) = NULL;
class option_box {
	private:
		float bottom_corner_x, bottom_corner_y;
		float bc;
		float height, width; 
		float color[3];
		char option_name[20];
	public:
		option_box(){
			bottom_corner_x = 0;
			bottom_corner_y = 0;
			width = 0;
			height = 0;
			color[RED] = 0.0;
			color[GREEN] = 0.0;
			color[BLUE] = 1.0;
			strcpy(option_name, "");
		}
		option_box(const char name[]){
			bottom_corner_x = 150;
			bottom_corner_y = 700;
			width = 150;
			height = 50;
			color[RED] =0.36 ;
			color[GREEN] =  0.25 ;
			color[BLUE] =  0.20;
			strcpy(option_name, name);
		}
		void draw_box(){
			bottom_corner_x += offset;
			bc = bottom_corner_x;
			glColor3f(color[RED], color[GREEN], color[BLUE]);
			glBegin(GL_POLYGON);
				glVertex2f(bottom_corner_x, bottom_corner_y);
				glVertex2f(bottom_corner_x + width, bottom_corner_y);
				glVertex2f(bottom_corner_x + width, bottom_corner_y + height);
				glVertex2f(bottom_corner_x, bottom_corner_y + height);
			glEnd();
			offset += 200.0;
			glColor3f(1.0, 1.0, 0.0);
			display_string(option_name, bottom_corner_x + 30.0, bottom_corner_y + 20.0);
		}
		bool clicked(int x, int y){
			if( x >=bc&& x < bc+width)
				if( y > bottom_corner_y && y < bottom_corner_y + height){
					box_clicked = true;
					return true;
				}
			return false;
		}
};
option_box insert_box, search_box, delete_box,preorder_box,inorder_box,postorder_box; 
void init() {
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0,1500,0.0,800);
}
void displayOptions(){
 	option_box insert_box("Insert"), search_box("Search"), delete_box("Delete"),preorder_box("Preorder"),inorder_box("Inorder"),postorder_box("Postorder");
 	offset = 0.0;
	insert_box.draw_box();
	search_box.draw_box();
	delete_box.draw_box();
	preorder_box.draw_box();
	inorder_box.draw_box();
	postorder_box.draw_box();
	::insert_box = insert_box;
	::search_box = search_box;
	::delete_box = delete_box;
	::preorder_box=preorder_box;
	::inorder_box=inorder_box;
	::postorder_box=postorder_box;
	
 }
void display() {
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	displayOptions();
	glFlush();
}
void mouse(int button, int state, int x, int y){
	y = 800- y;
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		if(prev_search == true){
			glColor3f(node_color[RED], node_color[GREEN], node_color[BLUE]);
			tree.draw_node(temp_item, temp_x, temp_y);
			prev_search = false;
		}
		if(insert_box.clicked(x, y)){
			operation = &binary_search_tree::insert;
			clear_input_region();
			glColor3f(1.0, 1.0, 0.0);
			display_string(query_string, 100, 650);
			glFlush();
		}
		if(search_box.clicked(x, y)){
			operation = &binary_search_tree::search;
			clear_input_region();
			glColor3f(1.0, 1.0, 0.0);
			display_string(query_string2, 100, 650);
			glFlush();
		}
		if(delete_box.clicked(x, y)){
			operation = &binary_search_tree::remove;
			clear_input_region();
			glColor3f(1.0, 1.0, 0.0);
			display_string(query_string3, 100, 650);
			glFlush();
		}
		if(preorder_box.clicked(x, y)){
			i=0;
			tree.preordertraversal();
		}
		if(inorder_box.clicked(x, y)){
			i=0;
			tree.inordertraversal();
		}
		if(postorder_box.clicked(x, y)){
			i=0;
			tree.postordertraversal();
		}
	}
		
}
void keyboard(unsigned char key, int x, int y){
	char s[2];
	static int digit = 0;
	static int number = 0;
	static int sign = 1;
          int l=0;
	if(!box_clicked)
		return ;
	if(digit == 0 && key == '-'){
		s[0]=key;
		s[1]='\0';
		glColor3f(1.0, 1.0, 0.0);
		display_string(s, 570,650);
		glFlush();
		sign = -1;
	}
	else if(key != 13 && isdigit(key)){
		number = (number*10) + key - '0';
		s[0]=key;
		s[1]='\0';
		glColor3f(1.0, 1.0, 0.0);
		display_string(s, 570+(digit*10), 650);
		digit++;
	}
	else if(key != 13 && !isdigit(key))
		;
	else{
		digit = 0;
		clear_input_region();
		(tree.*operation)(sign*number);
		number = 0;
		sign = 1;
		box_clicked = false;
	}
}
int main(int argc,char **argv) {	
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(1500,800);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Binary Search Tree Visualization");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	init();
	glutMainLoop();
	return 0;
}
