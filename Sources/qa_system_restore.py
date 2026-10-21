from pathlib import Path
import subprocess,tempfile,signal,time
root=Path.cwd();tools=root/'package/APPS/SonicMania_RGDSPlus/tools'
main=(root/'package/APPS/SonicMania.sh').read_text();chunk=main[main.index('. "$app_dir/tools/screen-control.sh"'):]
for mode,initial in [('normal',0),('normal',3),('normal',10),('error',3),('signal',3)]:
 with tempfile.TemporaryDirectory() as temp:
  d=Path(temp);(d/'tools').mkdir();light=d/'brightness';volume=d/'volume'
  light.write_text('70\n');volume.write_text(str(initial)+'\n')
  for name in ['screen-control.sh','volume-control.sh','screen-guard.sh']:
   text=(tools/name).read_text().replace('/sys/class/backlight/backlight/brightness',str(light)).replace('/sys/class/anbernic_misc/openbor_volume',str(volume))
   (d/'tools'/name).write_text(text)
  body='exec sleep 20' if mode=='signal' else 'sleep 0.05\nexit '+('7' if mode=='error' else '0')
  (d/'SonicMania').write_text(f'#!/bin/bash\necho 0 > "{light}"\necho 7 > "{volume}"\n{body}\n');(d/'SonicMania').chmod(0o755)
  (d/'run.sh').write_text('#!/bin/bash\nset -u\napp_dir="'+str(d)+'"\ncd "$app_dir"\n'+chunk)
  p=subprocess.Popen(['bash',str(d/'run.sh')],stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True)
  if mode=='signal':
   for i in range(200):
    if volume.read_text().strip()=='7':break
    time.sleep(.01)
   else:raise AssertionError('child did not start')
   p.send_signal(signal.SIGTERM)
  out,_=p.communicate(timeout=10)
  assert p.returncode=={'normal':0,'error':7,'signal':143}[mode],out
  assert light.read_text().strip()=='70',(mode,out)
  assert volume.read_text().strip()==str(initial),(mode,out,volume.read_text())
  assert f'restored system level={initial}' in out
  print('PASS:',mode,'volume',initial,'->7->',initial,'and brightness 70 restored',flush=True)
for screen_enabled in [True,False]:
 with tempfile.TemporaryDirectory() as temp:
  d=Path(temp);light=d/'light';volume=d/'volume';light.write_text('0');volume.write_text('9')
  child=subprocess.Popen(['sleep','20'])
  p=subprocess.Popen(['setsid','bash',str(tools/'screen-guard.sh'),str(child.pid),'70' if screen_enabled else '',str(light),'3',str(volume)])
  child.kill();child.wait();assert p.wait(timeout=3)==0
  assert volume.read_text().strip()=='3'
  assert light.read_text().strip()==('70' if screen_enabled else '0')
  print('PASS: independent guardian after forced child death; screen=',screen_enabled,flush=True)
for invalid in ['-1','11','bad','999999999999999999999999999']:
 with tempfile.TemporaryDirectory() as temp:
  node=Path(temp)/'volume';node.write_text(invalid+'\n')
  script=(tools/'volume-control.sh').read_text().replace('/sys/class/anbernic_misc/openbor_volume',str(node))
  p=subprocess.run(['bash','-c',script+'\nrgds_volume_save\n[ -z "$rgds_volume_saved" ]\necho 8 > "$rgds_volume_node"\nrgds_volume_restore\n'],capture_output=True,text=True)
  assert p.returncode==0 and node.read_text().strip()=='8'
print('PASS: invalid snapshots never restore a guessed value',flush=True)
