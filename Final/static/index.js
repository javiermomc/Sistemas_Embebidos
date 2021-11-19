light = false;

function toggleLight(){
    data = {
        request: 'light',
        body: ''
    }
    data.body = !light;
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function(data) {
        if (this.readyState == 4 && this.status == 200) {
            light = JSON.parse(this.responseText);
            if(!light && document.querySelector('.bulb.on').className.search('hide') >= 0){
                document.querySelector('.bulb.txt.off').classList.add('hide');
                document.querySelector('.bulb.txt.on').classList.remove('hide');
                document.querySelector('.bulb.img.off').classList.add('hide');
                document.querySelector('.bulb.img.on').classList.remove('hide');
                document.querySelector('#bulb-item').style.backgroundColor = '#9ACD32';
            }else if(light && document.querySelector('.bulb.off').className.search('hide') >= 0){
                document.querySelector('.bulb.txt.on').classList.add('hide');
                document.querySelector('.bulb.txt.off').classList.remove('hide');
                document.querySelector('.bulb.img.on').classList.add('hide');
                document.querySelector('.bulb.img.off').classList.remove('hide');
                document.querySelector('#bulb-item').style.backgroundColor = '#FFFFFF';
            }
        }
    };
    xhttp.open("POST", "/manager", true);
    xhttp.setRequestHeader("Content-type", "application/json; charset=utf-8");
    xhttp.send(JSON.stringify(data));
}

function changeLCD(){
    data = {
        request: 'text',
        body: document.querySelector('#disp-top').value + '\n' + document.querySelector('#disp-bottom').value
    }
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        console.log()
        if (this.readyState == 4 && this.status == 200) {
            if(JSON.parse(this.responseText) == true){
                document.querySelector('.check.disp').classList.toggle('hide', false);
                document.querySelector('.error.disp').classList.toggle('hide', true);
                document.querySelector('.spinner.disp').classList.toggle('hide', true);
            } else {
                document.querySelector('.check.disp').classList.toggle('hide', false);
                document.querySelector('.error.disp').classList.toggle('hide', true);
                document.querySelector('.spinner.disp').classList.toggle('hide', true);
            }
        } else {
            document.querySelector('.check.disp').classList.toggle('hide', true);
            document.querySelector('.error.disp').classList.toggle('hide', true);
            document.querySelector('.spinner.disp').classList.toggle('hide', false);
        } 
    };
    xhttp.onerror = function(e){
        console.log('ERROR')
        document.querySelector('.check.disp').classList.toggle('hide', true);
        document.querySelector('.error.disp').classList.toggle('hide', false);
        document.querySelector('.spinner.disp').classList.toggle('hide', true);
    }
    xhttp.open("POST", "/manager", true);
    xhttp.setRequestHeader("Content-type", "application/json; charset=utf-8");
    xhttp.send(JSON.stringify(data));
}

document.querySelector('#disp-top').oninput = changeLCD;
document.querySelector('#disp-bottom').oninput = changeLCD;

timeError = 0;

function updateTime(){
    data = {
        request: 'time'
    }
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.querySelector('#time').innerHTML = JSON.parse(this.responseText);
        }
    };
    xhttp.onerror = function(e){
        console.log('TIME ERROR');
        if(timeError>5){
            console.log('CONNECTION WITH TIME CLOSED');
            clearInterval(RTCLoop);
        }
        timeError++;
    }
    xhttp.open("POST", "/manager", true);
    xhttp.setRequestHeader("Content-type", "application/json; charset=utf-8");
    xhttp.send(JSON.stringify(data));
}
var RTCLoop = setInterval(updateTime, 1000);

window.addEventListener('DOMContentLoaded', (event) => {
    document.querySelector('.check.disp').classList.toggle('hide', true);
    document.querySelector('.error.disp').classList.toggle('hide', true);
    document.querySelector('.spinner.disp').classList.toggle('hide', true);
});