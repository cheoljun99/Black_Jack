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
{// 카드 섞어 주는 함수
	if (!card_deck->empty())	//덱이 비어있지 않다면
		card_deck->clear();	// 남은 카드를 모두 삭제해야한다.

	for (int i = 0; i < 12; i++) {
		card_deck->push_back(1);	//A부터
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
		card_deck->push_back(13);	//K까지
	}
	std::random_shuffle(card_deck->begin(), card_deck->end());
}


// 카드 숫자 합이 얼마인지 계산하는 함수
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
		{	//A인 경우
			a_count++;	//A의 개수를 세어줌
		}
		else if (hand[i] >= 10)
		{
			//10이상
			sum += 10;
		}
		else 
		{	
			sum += hand[i];
		}
	}
	// A를 1로 사용할지 11로 사용할지 판단하는 과정 Greedy Algorithm 을 사용한다.
	for (int i = 0; i < a_count + 1; i++)
	{	// A의 조합을 활용 (p, 1-p) 개수만큼 반복
		for (int j = 0; j < a_count - i; j++)
		{	
			a_sum += 11;
		}
		for (int j = 0; j < i; j++) 
		{	
			a_sum += 1;
		}
		if (sum + a_sum > 21)
		{	// 기존 합과 a의 합이 21보다 크다면
			//bust이기 때문에 a_sum을 다시 구해준다.
			a_sum = 0;
		}
		else
		{
			bust_check_flag = false; //bust하지 않은 경우
			sum += a_sum;
			break;	
		}
	}
	if (bust_check_flag == true)
	{	//a를 모두 1로 해도 bust인 경우 
		for (int i = 0; i < a_count; i++)
		{
			sum += 1;	//모든 값을 1로 더해주고 리턴한다.
		}
	}
	return sum;	//카드의 합을 반환.
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
		{	//A인 경우
			sum += 11;	//A의 개수를 세어줌
		}
		else if (hand[i] >= 10)
		{
			//10이상
			sum += 10;
		}
		else
		{
			sum += hand[i];
		}
	}

	return sum;	//카드의 합을 반환.
}


double CardCounting_for_betting(int card_count[14])
{
	//베팅할 돈을 위해 카드를 카운팅하는 함수.
	int total_used = 0;
	int total = 156;
	int over_ten_used = 0;
	int two_to_six_used = 0;
	double percentage = 0.0;

	for (int i = 1; i < 14; i++) {
		total_used += card_count[i];	//전체 사용한 카드 개수
	}

	for (int i = 10; i < 14; i++) {	//10에 근접한 A도 카운팅한다.
		over_ten_used += card_count[i];	//10, J, Q, K 사용한 개수
	}
	over_ten_used += card_count[1];	//A개수

	for (int i = 2; i < 7; i++) {
		two_to_six_used += card_count[i];	// 2, 3, 4, 5, 6 사용한 개수
	}

	//Card counting 전략

	//게임 시작시 돈을 걸때는 카드를 받기 이전이므로 딜러의 두번째 카드 즉 플레이어가 볼 수 있는 카드에 대한 존재를 모르기 때문에
	//정확한 카드 카운팅이 불가능하다 생각되기에
	//돈을 걸때는 Hit,stand판단과는 다르게 하이 로우 카운팅 기법을 사용하여 돈을 걸 액수를 정한다.
	//즉 이 함수는 돈을걸 액수를 정할 betting함수에서 실행되며 하이 로우 카운팅으로 딜러의 패가 10이상의 높은 카드가 나올 확율을 반환한다. 
	percentage = ((double)two_to_six_used - (double)over_ten_used) / ((((double)total - (double)total_used))/52);


	return percentage;
}

double CardCounting_for_decting_dealer_bust(int card_count[14],int second_card_of_dealer)
{
	//딜러의 버스트확률을 구하는 함수.
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
		ext_card = ext_card + (((12 - card_count[i])) * 10);// 10이상 K이하의 그동안 나오지 않은 카드를 개수를 저장한다.


	}
	for (int i = 1; i < 10; i++)
	{
		total_used += card_count[i];
		ext_card = ext_card + (((12 - card_count[i])) * i);// A이상 9이하의 그동안 나오지 않은 카드를 개수를 저장한다.


	}
	ext_card = ext_card / ((double)total - (double)total_used);// 나올 카드의 기대값을 구하는 과정이다. 
	ext_card = round(ext_card);//카드 값은 소수로 나오면 안됨으로 반올림해준다.
	if (ext_card == 1)
	{
		//기대한 카드가 A이면 딜러 기준 11임으로 기대값을 변환해준다.
		ext_card = 11; 
	}
	//딜러의 두번째 카드는 알고 있으므로
	//기대값과 딜러의 두번째 카드를 더해주어 버스트하는 값의 가장작은 값을 구하는 과정.
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
	bust_point= 21-sum_of_ext_card_and_second_card_of_dealer+1;//버스트 하는 카드값들의 가장 작은 값을 구한다.
	if (bust_point >= 10 && bust_point <= 13)
	{
		bust_point = 10;
	}
	if (bust_point <= 1)
	{
		//버스트 하는 카드값의 가장작은 값이 1이하이면 무조건 버스트
		percentage_bust = 100;
		return percentage_bust;
	}
	else if (bust_point > 10)
	{
		//버스트 하는 카드의 가장 작은 값이 10보다 크다면 무조건 히트임으로
		percentage_bust = 0;
		return percentage_bust;
	}
	for (int i = bust_point; i < 14; i++) {	
		//bust_point부터 K까지 그리고 A까지 사용한 카드 개수.
		dealer_over_bust_used += card_count[i];	
	}
	dealer_over_bust_used += card_count[1];	// + A 사용 개수
	for (int i = 2; i < bust_point; i++) 
	{
		dealer_under_bust_used += card_count[i];	// 2부터 bust_point미만까지 사용한 카드 개수
	}
	//딜러가 버스트 할 확률
	percentage_bust = ((3*(13-bust_point+2)*4)- dealer_over_bust_used) / (((double)total - (double)total_used));
	percentage_bust = percentage_bust * 100;
	return percentage_bust;
}

double CardCounting_for_decting_counting_player_bust(int card_count[14], int hand[17])
{
	//카드카운팅 플레이어의 버스트 할 확률을 구하는 함수. 
	//cout << "========================counting_player===================================" << endl;
	int total_used = 0;
	int total = 156;
	int counting_player_over_bust_used = 0;
	int counting_player_under_bust_used = 0;
	double percentage_bust = 0.0;
	int counting_player_hand_sum = 0;
	int bust_point = 0;
	counting_player_hand_sum = SumofNum(hand);// 카드카운팅 플레이어의 손에 있는 카드를 다 더해 저장한다.
	bust_point = 21 - counting_player_hand_sum + 1;// 버스트를 예상 하는 카드의 가장작은 경우를 구해 저장
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
		//버스트 예상 하는 카드의 가장작은 경우가 1포함 작은 경우 무조건 버스트함으로
		percentage_bust = 100;
		return percentage_bust;
	}
	else if (bust_point > 10)
	{
		//버스트 하는 카드의 가장 작은 값이 10보다 크다면 무조건 히트임으로
		percentage_bust = 0;
		return percentage_bust;
	}
	for (int i = bust_point; i < 14; i++)
	{
		//bust_point부터 K까지 그리고 A까지 사용한 카드 개수.
		counting_player_over_bust_used += card_count[i];
	}
	for (int i = 1; i < bust_point; i++)
	{
		counting_player_under_bust_used += card_count[i];	// 2부터 bust_point미만까지 사용한 카드 개수
	}
	percentage_bust = ((3 * (13 - bust_point + 1) * 4) - counting_player_over_bust_used) / (((double)total - (double)total_used));
	percentage_bust = percentage_bust * 100;
	return percentage_bust;
}

// 베팅 금액 설정 함수
double Betting(int card_count[14], bool counting_flag)
{
	double hi_lo_counting = 0.0;
	if (counting_flag) {	
		//카운팅 플레이어는 확률에 따라 베팅한다.


		hi_lo_counting = CardCounting_for_betting(card_count);
		if ((int)hi_lo_counting<=0)
		{
			
			//하이 로우 카운팅 값이 0또는 그 이하일 때 딜러의 버스트 확률이 가장 작다 그러므로 돈을 가장 적게 건다.
			return 100;
		}
		else if ((int)hi_lo_counting > 0 && (int)hi_lo_counting <= 10)
		{
			//하이 로우 카운팅 값이 0보다 크고 10이하일 때 하이 로우 카운팅 값에 따라 돈을 건다.
			return (int)hi_lo_counting * 100;
		}
		else
		{
			//딜러의 버스트 확률이 매우 크므로
			return 1000;
		}

	}
	else 
	{	
		//simple player는 500원씩 베팅한다.
		return 500;
	}
}


// Hit할지 Stand할지 선택하는 함수
bool HitStatus(int hand[17], int card_count[14], int second_card_of_dealer, bool counting_flag)
{

	double dealer_bust_percentage = 0.0;
	double counting_player_bust_percentage = 0.0;
	if (counting_flag)
	{	
		//card counting player의 경우
		//카드 카운팅 사용
		dealer_bust_percentage = CardCounting_for_decting_dealer_bust(card_count, second_card_of_dealer);//딜러의 버스트 확률 구함.
		counting_player_bust_percentage = CardCounting_for_decting_counting_player_bust(card_count, hand);// 카운팅 플레이어의 버스트 확률 구함.
		if (SumofNum(hand) >= 17) 
		{
			return true;	//stand 반환
		}
		else
		{
			//손에 있는 카드합이 17이하인 경우
			if (counting_player_bust_percentage > 40)
			{
				return true;	//플레이어가 버스트 할 확률이 40보다 크다면 stand
			}
			else
			{
				// 플레이어가 버스트 할 확률이 50%보다 작을때
				if (counting_player_bust_percentage == 0) 
				{	
					//플레이어가 버스트 할 확률이 0이면  담음카드에 모든걸 걸어본다.
					
					return false;	//hit한다.
				}
				if (2 <= second_card_of_dealer && second_card_of_dealer <= 5)
				{
					// A 가 첫번째로 나와도 무조건 hit을 해야 한다. 
					// 이때 10 이상의 카드가 나올 확률이 50%가 넘는다면 
					// 딜러가 스스로 bust할 확률이 높으므로 player는 stand하게 된다.
					if (dealer_bust_percentage >= 40) 
					{
						//cout << "ㅎㅇ" << endl;
						//딜러가 버스트 할 확률이 40%포함 크다면 stand 
						return true;	//stand를 하여 딜러의 bust를 기대해본다.
					}
				}
				return false;
			}
		}
	}
	else
	{	//simple player & dealer 의 경우
		if (SumofNum(hand) >= 17)
		{
			return true;	//stand 반환
		}
		else return false;	//hit 반환
	}
}


// 블랙잭 메인 함수
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
		//입력한수만큼 진행
		// 
		//80% 이상 사용했으면 Shuffle
		if (card_deck.size() < 32)
		{		//80%이상 사용되었다면
			for (int i = 0; i < 14; i++)
			{
				card_count[i] = 0;	//카운팅 초기화
			}
			Shuffle(&card_deck);	//Shuffle
		}

		// 딜러 ,플레이어 카드 초기화
		for (int i = 0; i < 17; i++)
		{
			simple_hand[i] = 0;
			counting_hand[i] = 0;
			dealer_hand[i] = 0;
		}
		// 플레이어 배팅 금액 설정
		simple_player_betting = Betting(card_count, false);
		counting_player_betting = Betting(card_count, true);

		// 카드를 2장씩 번갈아 받는다.

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


		
		// Card Counting player 게임 시작 -> 카드 카운팅 하여 hit or stand한다.
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
		card_count[first_card_of_dealer]++; //딜러의 첫번째 패를 카운팅 함
		first_card_of_dealer = 0;

		// Simple player 게임 시작 -> 딜러 규칙대로 진행한다.
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
		// 딜러 게임 시작
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

		// 플레이어들의 카드가 21보다 크다면? 딜러의 승으로 게임 끝
		if (SumofNum(counting_hand) > 21) {
			counting_p->budget -= counting_player_betting;	// 베팅금액만큼 잃는다,
			counting_p->rose += 1;	//rose count
			counting_bust_check = true;
		}
		if (SumofNum(simple_hand) > 21) {
			simple_p->budget -= simple_player_betting;
			simple_p->rose += 1;//rose count
			simple_bust_check = true;
		}

		// 딜러의 카드가 플레이어 보다 클때

		counting_card_sum = SumofNum(counting_hand);
		simple_card_sum = SumofNum(simple_hand);
		dlealer_card_sum = SumofNum_for_dealer(dealer_hand);
		
		
		// 카운팅 플레이어 먼저
		if (counting_bust_check == false)
		{	
			//카운팅 플레이어가 bust하지 않았을 경우
			if (dlealer_card_sum > counting_card_sum)
			{
				if (dlealer_card_sum > 21)
				{
					// 카운팅 플레이어 승
					counting_p->budget += counting_player_betting;	// 베팅금액 2배만큼 얻는다.
					counting_p->win += 1;	//win count
				}
				else {	
					//카운팅 플레이어 짐
					counting_p->budget -= counting_player_betting;	// 베팅금액만큼 잃는다,
					counting_p->rose += 1;	//rose count
				
				}
			}
			else if (dlealer_card_sum == counting_card_sum)
			{	
				//무승부
				counting_p->draw += 1;	//draw count
			}
			else 
			{	
				// 카운팅 블랙잭으로 플레이어 승
				if (counting_card_sum == 21)
				{
					counting_p->budget += 1.5*counting_player_betting;	// 베팅금액의 2.5배 만큼 얻는다.
					counting_p->win += 1;
				}
				else
				{
					counting_p->budget += counting_player_betting;	// 베팅금액만큼 얻는다.
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

	int game_count = 0;		//게임 횟수
	cout << "Play Game : ";
	cin >> game_count;	
	srand((unsigned int)time(NULL));

	struct player player_card_counting;
	player_card_counting.budget = 100000.0;	//player의 예산 10만원으로 초기자본 부여
	player_card_counting.win = 0;//승리 횟수
	player_card_counting.draw = 0;//비긴 횟수
	player_card_counting.rose = 0;//진 횟수

	struct player player_simple;//player설정
	player_simple.budget = 100000.0;//player의 예산 10만원으로 초기자본 부여
	player_simple.win = 0;	//승리 횟수
	player_simple.draw = 0;	//비긴 횟수
	player_simple.rose = 0; //진 횟수
	playgame(game_count, &player_simple, &player_card_counting);

	cout << "<< Player1 >>\n"
		<< "승리 : " << player_card_counting.win
		<< " 무승부 : " << player_card_counting.draw
		<< " 패배 : "<<player_card_counting.rose << endl
		<< "승률 : " << 100 * ((double)player_card_counting.win / (double)game_count)
		<< "%\n"
		<<"money : " << player_card_counting.budget << endl;

	cout << endl;

	cout << "<< Player2 >>\n"
		<< "승리 : " << player_simple.win
		<< " 무승부 : " << player_simple.draw
		<< " 패배 : " << player_simple.rose << endl
		<< "승률 : " << 100 * ((double)player_simple.win / (double)game_count)
		<< "%\n"
		<< "money : " << player_simple.budget << endl;
	return 0;

}