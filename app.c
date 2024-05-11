void PAPP::SetGameSpeed(int speed)
{
	gamespeed = 1000/speed;
}

void PAPP::SetActive(BOOL b)
{
	active = b;
}

void PAPP::SetPause(BOOL b)
{
	pause = b;
}

BOOL PAPP::IsActive()
{
	return active;
}