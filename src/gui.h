bool handleButtons(float x, float y);
void drawButtons();

vec3 buttonT = vec3(0.0,1.0,0.0);
vec3 buttonB = vec3(0.0,0.0,1.0);
vec3 textC = vec3(0.0,0.0,0.0);


Button translateButton = Button("Translate",vec2(0.0,0.0),true,false,textC,vec4(10.0,10.0,10.0,10.0),buttonT, buttonB, true,true);
Button rotateButton = Button("Rotate",vec2(60.0,0.0),true,false,textC,vec4(10.0,10.0,10.0,10.0),buttonT, buttonB, false,true);
Button scaleButton = Button("Scale",vec2(120.0,3.0),true,false,textC,vec4(10.0,10.0,10.0,10.0),buttonT, buttonB, false,true);

bool handleButtons(float x, float y)
{
	if(translateButton.contains(x,y))
		transM = TRANS_TRANSLATION;;
	else
		return false;
	return true;
}

void drawButtons()
{
	translateButton.drawButton();
	rotateButton.drawButton();
	scaleButton.drawButton();
}
