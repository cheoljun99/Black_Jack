#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include<cmath>

using namespace std;

struct player {
	int budget;
	int win;
	int draw;
	int rose;
};



void Shuffle(vector<int>* card_deck)
{// ī�� ���� �ִ� �Լ�
	if (!card_deck->empty())	//���� ������� �ʴٸ�
		card_deck->clear();	// ���� ī�带 ��� �����ؾ��Ѵ�.

	for (int i = 0; i < 12; i++) {
		card_deck->push_back(1);	//A����
		card_deck->push_back(2);
		card_deck->push_back(3);
		card_deck->push_back(4);
		card_deck->push_back(5);
		card_deck->push_back(6);
		card_deck->push_back(7);
		card_deck->push_back(8);
		card_deck->push_back(9);
		card_deck->push_back(10);
		card_deck->push_back(11);	
		card_deck->push_back(12);	
		card_deck->push_back(13);	//K����
	}
	std::random_shuffle(card_deck->begin(), card_deck->end());
}


// ī�� ���� ���� ������ ����ϴ� �Լ�
int SumofNum(int hand[17])
{
	int sum = 0;
	int a_count = 0;
	int a_sum = 0;
	bool bust_check_flag = true;
	for (int i = 0; i < 17; i++)
	{
		if (hand[i] == 0) 
		{
			break;
		}
		else if (hand[i] == 1)
		{	//A�� ���
			a_count++;	//A�� ������ ������
		}
		else if (hand[i] >= 10)
		{
			//10�̻�
			sum += 10;
		}
		else 
		{	
			sum += hand[i];
		}
	}
	// A�� 1�� ������� 11�� ������� �Ǵ��ϴ� ���� Greedy Algorithm �� ����Ѵ�.
	for (int i = 0; i < a_count + 1; i++)
	{	// A�� ������ Ȱ�� (p, 1-p) ������ŭ �ݺ�
		for (int j = 0; j < a_count - i; j++)
		{	
			a_sum += 11;
		}
		for (int j = 0; j < i; j++) 
		{	
			a_sum += 1;
		}
		if (sum + a_sum > 21)
		{	// ���� �հ� a�� ���� 21���� ũ�ٸ�
			//bust�̱� ������ a_sum�� �ٽ� �����ش�.
			a_sum = 0;
		}
		else
		{
			bust_check_flag = false; //bust���� ���� ���
			sum += a_sum;
			break;	
		}
	}
	if (bust_check_flag == true)
	{	//a�� ��� 1�� �ص� bust�� ��� 
		for (int i = 0; i < a_count; i++)
		{
			sum += 1;	//��� ���� 1�� �����ְ� �����Ѵ�.
		}
	}
	return sum;	//ī���� ���� ��ȯ.
}

int SumofNum_for_dealer(int hand[17])
{
	int sum = 0;
	int a_count = 0;
	int a_sum = 0;
	bool bust_check_flag = true;
	for (int i = 0; i < 17; i++)
	{
		if (hand[i] == 0)
		{
			break;
		}
		else if (hand[i] == 1)
		{	//A�� ���
			sum += 11;	//A�� ������ ������
		}
		else if (hand[i] >= 10)
		{
			//10�̻�
			sum += 10;
		}
		else
		{
			sum += hand[i];
		}
	}

	return sum;	//ī���� ���� ��ȯ.
}


double CardCounting_for_betting(int card_count[14])
{
	//������ ���� ���� ī�带 ī�����ϴ� �Լ�.
	int total_used = 0;
	int total = 156;
	int over_ten_used = 0;
	int two_to_six_used = 0;
	double percentage = 0.0;

	for (int i = 1; i < 14; i++) {
		total_used += card_count[i];	//��ü ����� ī�� ����
	}

	for (int i = 10; i < 14; i++) {	//10�� ������ A�� ī�����Ѵ�.
		over_ten_used += card_count[i];	//10, J, Q, K ����� ����
	}
	over_ten_used += card_count[1];	//A����

	for (int i = 2; i < 7; i++) {
		two_to_six_used += card_count[i];	// 2, 3, 4, 5, 6 ����� ����
	}

	//Card counting ����

	//���� ���۽� ���� �ɶ��� ī�带 �ޱ� �����̹Ƿ� ������ �ι�° ī�� �� �÷��̾ �� �� �ִ� ī�忡 ���� ���縦 �𸣱� ������
	//��Ȯ�� ī�� ī������ �Ұ����ϴ� �����Ǳ⿡
	//���� �ɶ��� Hit,stand�Ǵܰ��� �ٸ��� ���� �ο� ī���� ����� ����Ͽ� ���� �� �׼��� ���Ѵ�.
	//�� �� �Լ��� ������ �׼��� ���� betting�Լ����� ����Ǹ� ���� �ο� ī�������� ������ �а� 10�̻��� ���� ī�尡 ���� Ȯ���� ��ȯ�Ѵ�. 
	percentage = ((double)two_to_six_used - (double)over_ten_used) / ((((double)total - (double)total_used))/52);


	return percentage;
}

double CardCounting_for_decting_dealer_bust(int card_count[14],int second_card_of_dealer)
{
	//������ ����ƮȮ���� ���ϴ� �Լ�.
	//cout << "========================dealer===================================" << endl;
	int total_used = 0;
	int total = 156;	// A ~ K
	int dealer_over_bust_used = 0;
	int dealer_under_bust_used = 0;
	double percentage_bust = 0.0;
	double ext_card = 0.0;
	int count = 0;
	int sum_of_ext_card_and_second_card_of_dealer = 0;
	int bust_point = 0;
	for (int i = 10; i < 14; i++)
	{
		total_used += card_count[i];
		ext_card = ext_card + (((12 - card_count[i])) * 10);// 10�̻� K������ �׵��� ������ ���� ī�带 ������ �����Ѵ�.


	}
	for (int i = 1; i < 10; i++)
	{
		total_used += card_count[i];
		ext_card = ext_card + (((12 - card_count[i])) * i);// A�̻� 9������ �׵��� ������ ���� ī�带 ������ �����Ѵ�.


	}
	ext_card = ext_card / ((double)total - (double)total_used);// ���� ī���� ��밪�� ���ϴ� �����̴�. 
	ext_card = round(ext_card);//ī�� ���� �Ҽ��� ������ �ȵ����� �ݿø����ش�.
	if (ext_card == 1)
	{
		//����� ī�尡 A�̸� ���� ���� 11������ ��밪�� ��ȯ���ش�.
		ext_card = 11; 
	}
	//������ �ι�° ī��� �˰� �����Ƿ�
	//��밪�� ������ �ι�° ī�带 �����־� ����Ʈ�ϴ� ���� �������� ���� ���ϴ� ����.
	if (second_card_of_dealer >= 10)
	{
		sum_of_ext_card_and_second_card_of_dealer = ext_card + 10;
	}
	else if(second_card_of_dealer == 1)
	{
		sum_of_ext_card_and_second_card_of_dealer = ext_card + 11;
	}
	else
	{
		sum_of_ext_card_and_second_card_of_dealer = ext_card + second_card_of_dealer;
	}
	bust_point= 21-sum_of_ext_card_and_second_card_of_dealer+1;//����Ʈ �ϴ� ī�尪���� ���� ���� ���� ���Ѵ�.
	if (bust_point >= 10 && bust_point <= 13)
	{
		bust_point = 10;
	}
	if (bust_point <= 1)
	{
		//����Ʈ �ϴ� ī�尪�� �������� ���� 1�����̸� ������ ����Ʈ
		percentage_bust = 100;
		return percentage_bust;
	}
	else if (bust_point > 10)
	{
		//����Ʈ �ϴ� ī���� ���� ���� ���� 10���� ũ�ٸ� ������ ��Ʈ������
		percentage_bust = 0;
		return percentage_bust;
	}
	for (int i = bust_point; i < 14; i++) {	
		//bust_point���� K���� �׸��� A���� ����� ī�� ����.
		dealer_over_bust_used += card_count[i];	
	}
	dealer_over_bust_used += card_count[1];	// + A ��� ����
	for (int i = 2; i < bust_point; i++) 
	{
		dealer_under_bust_used += card_count[i];	// 2���� bust_point�̸����� ����� ī�� ����
	}
	//������ ����Ʈ �� Ȯ��
	percentage_bust = ((3*(13-bust_point+2)*4)- dealer_over_bust_used) / (((double)total - (double)total_used));
	percentage_bust = percentage_bust * 100;
	return percentage_bust;
}

double CardCounting_for_decting_counting_player_bust(int card_count[14], int hand[17])
{
	//ī��ī���� �÷��̾��� ����Ʈ �� Ȯ���� ���ϴ� �Լ�. 
	//cout << "========================counting_player===================================" << endl;
	int total_used = 0;
	int total = 156;
	int counting_player_over_bust_used = 0;
	int counting_player_under_bust_used = 0;
	double percentage_bust = 0.0;
	int counting_player_hand_sum = 0;
	int bust_point = 0;
	counting_player_hand_sum = SumofNum(hand);// ī��ī���� �÷��̾��� �տ� �ִ� ī�带 �� ���� �����Ѵ�.
	bust_point = 21 - counting_player_hand_sum + 1;// ����Ʈ�� ���� �ϴ� ī���� �������� ��츦 ���� ����
	for (int i = 1; i < 14; i++)
	{
		total_used += card_count[i];
	}
	if (bust_point >= 10 && bust_point <= 13)
	{
		bust_point = 10;
	}
	if (bust_point <= 1)
	{
		//����Ʈ ���� �ϴ� ī���� �������� ��찡 1���� ���� ��� ������ ����Ʈ������
		percentage_bust = 100;
		return percentage_bust;
	}
	else if (bust_point > 10)
	{
		//����Ʈ �ϴ� ī���� ���� ���� ���� 10���� ũ�ٸ� ������ ��Ʈ������
		percentage_bust = 0;
		return percentage_bust;
	}
	for (int i = bust_point; i < 14; i++)
	{
		//bust_point���� K���� �׸��� A���� ����� ī�� ����.
		counting_player_over_bust_used += card_count[i];
	}
	for (int i = 1; i < bust_point; i++)
	{
		counting_player_under_bust_used += card_count[i];	// 2���� bust_point�̸����� ����� ī�� ����
	}
	percentage_bust = ((3 * (13 - bust_point + 1) * 4) - counting_player_over_bust_used) / (((double)total - (double)total_used));
	percentage_bust = percentage_bust * 100;
	return percentage_bust;
}

// ���� �ݾ� ���� �Լ�
double Betting(int card_count[14], bool counting_flag)
{
	double hi_lo_counting = 0.0;
	if (counting_flag) {	
		//ī���� �÷��̾�� Ȯ���� ���� �����Ѵ�.


		hi_lo_counting = CardCounting_for_betting(card_count);
		if ((int)hi_lo_counting<=0)
		{
			
			//���� �ο� ī���� ���� 0�Ǵ� �� ������ �� ������ ����Ʈ Ȯ���� ���� �۴� �׷��Ƿ� ���� ���� ���� �Ǵ�.
			return 100;
		}
		else if ((int)hi_lo_counting > 0 && (int)hi_lo_counting <= 10)
		{
			//���� �ο� ī���� ���� 0���� ũ�� 10������ �� ���� �ο� ī���� ���� ���� ���� �Ǵ�.
			return (int)hi_lo_counting * 100;
		}
		else
		{
			//������ ����Ʈ Ȯ���� �ſ� ũ�Ƿ�
			return 1000;
		}

	}
	else 
	{	
		//simple player�� 500���� �����Ѵ�.
		return 500;
	}
}


// Hit���� Stand���� �����ϴ� �Լ�
bool HitStatus(int hand[17], int card_count[14], int second_card_of_dealer, bool counting_flag)
{

	double dealer_bust_percentage = 0.0;
	double counting_player_bust_percentage = 0.0;
	if (counting_flag)
	{	
		//card counting player�� ���
		//ī�� ī���� ���
		dealer_bust_percentage = CardCounting_for_decting_dealer_bust(card_count, second_card_of_dealer);//������ ����Ʈ Ȯ�� ����.
		counting_player_bust_percentage = CardCounting_for_decting_counting_player_bust(card_count, hand);// ī���� �÷��̾��� ����Ʈ Ȯ�� ����.
		if (SumofNum(hand) >= 17) 
		{
			return true;	//stand ��ȯ
		}
		else
		{
			//�տ� �ִ� ī������ 17������ ���
			if (counting_player_bust_percentage > 40)
			{
				return true;	//�÷��̾ ����Ʈ �� Ȯ���� 40���� ũ�ٸ� stand
			}
			else
			{
				// �÷��̾ ����Ʈ �� Ȯ���� 50%���� ������
				if (counting_player_bust_percentage == 0) 
				{	
					//�÷��̾ ����Ʈ �� Ȯ���� 0�̸�  ����ī�忡 ���� �ɾ��.
					
					return false;	//hit�Ѵ�.
				}
				if (2 <= second_card_of_dealer && second_card_of_dealer <= 5)
				{
					// A �� ù��°�� ���͵� ������ hit�� �ؾ� �Ѵ�. 
					// �̶� 10 �̻��� ī�尡 ���� Ȯ���� 50%�� �Ѵ´ٸ� 
					// ������ ������ bust�� Ȯ���� �����Ƿ� player�� stand�ϰ� �ȴ�.
					if (dealer_bust_percentage >= 40) 
					{
						//cout << "����" << endl;
						//������ ����Ʈ �� Ȯ���� 40%���� ũ�ٸ� stand 
						return true;	//stand�� �Ͽ� ������ bust�� ����غ���.
					}
				}
				return false;
			}
		}
	}
	else
	{	//simple player & dealer �� ���
		if (SumofNum(hand) >= 17)
		{
			return true;	//stand ��ȯ
		}
		else return false;	//hit ��ȯ
	}
}


// ���� ���� �Լ�
void playgame(int game_count, struct player* simple_p, struct player* counting_p)
{
	int card_count[14] = { 0, };
	int temp, dlealer_card_sum, simple_card_sum, counting_card_sum, first_card_of_dealer, second_card_of_dealer;
	int dealer_hand[17] = { 0, };
	int simple_hand[17] = { 0, };
	int counting_hand[17] = { 0, };
	double simple_player_betting;
	double counting_player_betting;
	bool simple_bust_check = false, counting_bust_check = false;
	vector<int> card_deck;
	
	for (int j = 0; j < game_count; j++)
	{
		//�Է��Ѽ���ŭ ����
		// 
		//80% �̻� ��������� Shuffle
		if (card_deck.size() < 32)
		{		//80%�̻� ���Ǿ��ٸ�
			for (int i = 0; i < 14; i++)
			{
				card_count[i] = 0;	//ī���� �ʱ�ȭ
			}
			Shuffle(&card_deck);	//Shuffle
		}

		// ���� ,�÷��̾� ī�� �ʱ�ȭ
		for (int i = 0; i < 17; i++)
		{
			simple_hand[i] = 0;
			counting_hand[i] = 0;
			dealer_hand[i] = 0;
		}
		// �÷��̾� ���� �ݾ� ����
		simple_player_betting = Betting(card_count, false);
		counting_player_betting = Betting(card_count, true);

		// ī�带 2�徿 ������ �޴´�.

		temp = card_deck.back();
		card_deck.pop_back();
		dealer_hand[0] = temp;	
		first_card_of_dealer = temp;	

		temp = card_deck.back();
		card_deck.pop_back();
		counting_hand[0] = temp;	
		card_count[temp]++;

		temp = card_deck.back();
		card_deck.pop_back();
		simple_hand[0] = temp;	
		card_count[temp]++;

		temp = card_deck.back();
		card_deck.pop_back();
		dealer_hand[1] = temp;	
		second_card_of_dealer = temp;
		card_count[temp]++;	

		temp = card_deck.back();
		card_deck.pop_back();
		counting_hand[1] = temp;	
		card_count[temp]++;	

		temp = card_deck.back();
		card_deck.pop_back();
		simple_hand[1] = temp;	
		card_count[temp]++;


		
		// Card Counting player ���� ���� -> ī�� ī���� �Ͽ� hit or stand�Ѵ�.
		for (int i = 2; i < 17; i++)
		{
			if (HitStatus(counting_hand, card_count, second_card_of_dealer, true) == false)
			{ 
				temp = card_deck.back();
				card_deck.pop_back();
				counting_hand[i] = temp;	
				card_count[temp]++;	
			}
			else
				break;	//stand
		}
		card_count[first_card_of_dealer]++; //������ ù��° �и� ī���� ��
		first_card_of_dealer = 0;

		// Simple player ���� ���� -> ���� ��Ģ��� �����Ѵ�.
		for (int i = 2; i < 17; i++) {
			
			if (HitStatus(simple_hand, card_count, second_card_of_dealer, false) == false)
			{ 
				temp = card_deck.back();
				card_deck.pop_back();
				simple_hand[i] = temp;	
				card_count[temp]++;	
			}
			else
				break;	//stand
		}
		// ���� ���� ����
		for (int i = 2; i < 17; i++) {
			if (HitStatus(dealer_hand, card_count, second_card_of_dealer, false) == false)
			{ 
				temp = card_deck.back();
				card_deck.pop_back();
				dealer_hand[i] = temp;	
				card_count[temp]++;	
			}
			else
				break; //stand
		}

		// �÷��̾���� ī�尡 21���� ũ�ٸ�? ������ ������ ���� ��
		if (SumofNum(counting_hand) > 21) {
			counting_p->budget -= counting_player_betting;	// ���ñݾ׸�ŭ �Ҵ´�,
			counting_p->rose += 1;	//rose count
			counting_bust_check = true;
		}
		if (SumofNum(simple_hand) > 21) {
			simple_p->budget -= simple_player_betting;
			simple_p->rose += 1;//rose count
			simple_bust_check = true;
		}

		// ������ ī�尡 �÷��̾� ���� Ŭ��

		counting_card_sum = SumofNum(counting_hand);
		simple_card_sum = SumofNum(simple_hand);
		dlealer_card_sum = SumofNum_for_dealer(dealer_hand);
		
		
		// ī���� �÷��̾� ����
		if (counting_bust_check == false)
		{	
			//ī���� �÷��̾ bust���� �ʾ��� ���
			if (dlealer_card_sum > counting_card_sum)
			{
				if (dlealer_card_sum > 21)
				{
					// ī���� �÷��̾� ��
					counting_p->budget += counting_player_betting;	// ���ñݾ� 2�踸ŭ ��´�.
					counting_p->win += 1;	//win count
				}
				else {	
					//ī���� �÷��̾� ��
					counting_p->budget -= counting_player_betting;	// ���ñݾ׸�ŭ �Ҵ´�,
					counting_p->rose += 1;	//rose count
				
				}
			}
			else if (dlealer_card_sum == counting_card_sum)
			{	
				//���º�
				counting_p->draw += 1;	//draw count
			}
			else 
			{	
				// ī���� �������� �÷��̾� ��
				if (counting_card_sum == 21)
				{
					counting_p->budget += 1.5*counting_player_betting;	// ���ñݾ��� 2.5�� ��ŭ ��´�.
					counting_p->win += 1;
				}
				else
				{
					counting_p->budget += counting_player_betting;	// ���ñݾ׸�ŭ ��´�.
					counting_p->win += 1;

				}
				
			}
		}
		if (simple_bust_check == false)
		{	
			if (dlealer_card_sum > simple_card_sum)
			{
				if (dlealer_card_sum > 21)
				{
					
					simple_p->budget += simple_player_betting;	
					simple_p->win += 1;	//win count
				}
				else
				{	

					simple_p->budget -= simple_player_betting;	
					simple_p->rose += 1;//rose count

					
				}
			}
			else if (dlealer_card_sum == simple_card_sum)
			{	
				simple_p->draw += 1;	//draw count
			}
			else 
			{
			
				if (simple_card_sum == 21)
				{
					simple_p->budget += 1.5*simple_player_betting;	
					simple_p->win += 1;
				}
				else
				{

					simple_p->budget += simple_player_betting;	
					simple_p->win += 1;

				}
			}
		}
		counting_bust_check = false;
		simple_bust_check = false;
	}
}


int main()
{

	int game_count = 0;		//���� Ƚ��
	cout << "Play Game : ";
	cin >> game_count;	
	srand((unsigned int)time(NULL));

	struct player player_card_counting;
	player_card_counting.budget = 100000.0;	//player�� ���� 10�������� �ʱ��ں� �ο�
	player_card_counting.win = 0;//�¸� Ƚ��
	player_card_counting.draw = 0;//��� Ƚ��
	player_card_counting.rose = 0;//�� Ƚ��

	struct player player_simple;//player����
	player_simple.budget = 100000.0;//player�� ���� 10�������� �ʱ��ں� �ο�
	player_simple.win = 0;	//�¸� Ƚ��
	player_simple.draw = 0;	//��� Ƚ��
	player_simple.rose = 0; //�� Ƚ��
	playgame(game_count, &player_simple, &player_card_counting);

	cout << "<< Player1 >>\n"
		<< "�¸� : " << player_card_counting.win
		<< " ���º� : " << player_card_counting.draw
		<< " �й� : "<<player_card_counting.rose << endl
		<< "�·� : " << 100 * ((double)player_card_counting.win / (double)game_count)
		<< "%\n"
		<<"money : " << player_card_counting.budget << endl;

	cout << endl;

	cout << "<< Player2 >>\n"
		<< "�¸� : " << player_simple.win
		<< " ���º� : " << player_simple.draw
		<< " �й� : " << player_simple.rose << endl
		<< "�·� : " << 100 * ((double)player_simple.win / (double)game_count)
		<< "%\n"
		<< "money : " << player_simple.budget << endl;
	return 0;

}