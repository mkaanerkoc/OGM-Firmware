float calculateFlowFromHeight(uint16_t ref_height,uint16_t distance){
  
  float diff_cm =  ((float)(ref_height - distance)) / 10;
  float flow = diff_cm * diff_cm * diff_cm * diff_cm * diff_cm;
  flow = sqrt(flow) * 1.1917;
  return flow;
  
}


